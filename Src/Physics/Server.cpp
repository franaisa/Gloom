/**
@file Server.cpp

Contiene la implementación del servidor de física. 

@see Physics::CServer

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "Server.h"
#include "Conversions.h"
#include "ErrorManager.h"
#include "CollisionManager.h"
#include "Logic/Entity/Components/Physics.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Fluid.h"
#include "Cloth.h"
#include "MaterialManager.h"
#include "GeometryFactory.h"

#include <assert.h>
#include <algorithm>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h>
#include <extensions\PxVisualDebuggerExt.h> 
#include <RepX\RepX.h>

using namespace Logic;
using namespace physx;

namespace Physics {

	// Única instancia del servidor
	CServer* CServer::_instance = NULL;

	//________________________________________________________________________

	CServer::CServer() : _cudaContextManager(NULL), _scene(NULL) {
		// Crear gestor de errores
		_errorManager = new CErrorManager();

		// Crear gestor de memoria
		_allocator = new PxDefaultAllocator();

		// Crear gestor de colisiones
		_collisionManager = new CCollisionManager();

		// Crear PxFoundation. Es necesario para instanciar el resto de objetos de PhysX
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorManager);
		assert(_foundation && "Error en PxCreateFoundation");

		// Crear PxProfileZoneManager. Es necesario para habitiar algunas opciones de 
		// profiling de rendimiento en el PhysX Visual Debugger 
		_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(_foundation);
		assert(_profileZoneManager && "Error en PxProfileZoneManager::createProfileZoneManage");

		// Crear PxPhysics. Es el punto de entrada al SDK de PhysX
		PxTolerancesScale toleranceScale;
		bool recordMemoryAllocations = true;
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, toleranceScale, 
								   recordMemoryAllocations, _profileZoneManager);
		assert(_physics && "Error en PxCreatePhysics");

		// Crear CudaContextManager. Permite aprovechar la GPU para hacer parte de la simulación física.
		// Se utiliza posteriormente al crear la escena física.
		// Sólo Windows
	#ifdef PX_WINDOWS

		pxtask::CudaContextManagerDesc cudaContextManagerDesc;
		_cudaContextManager = pxtask::createCudaContextManager(*_foundation, cudaContextManagerDesc, 
															   _profileZoneManager);
		if(_cudaContextManager) {
			if( !_cudaContextManager->contextIsValid() ) {
				_cudaContextManager->release();
				_cudaContextManager = NULL;
			}
		}

	#endif

		// Crear PxControllerManager. Es necesario para crear character controllers
		_controllerManager = PxCreateControllerManager(*_foundation);

		// Inicializar el módulo PxCooking. Es necesario para cocinar mallas y para 
		// deserializar actores a partir de ficheros RepX
		PxCookingParams params;
		_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, params);

		// Intentar conectar con PhysX Visual Debugger (PVD)
		_pvdConnection = NULL;

		// Sólo en modo DEBUG
	#ifdef _DEBUG
		debugger::comm::PvdConnectionManager *pvdConnectionManager = _physics->getPvdConnectionManager();
		assert(pvdConnectionManager && "Error en PxPhysics::getPvdConnectionManager");

		// Configurar máquina, puerto y tiempo de espera (en millisegundos)
		const char *ip = "127.0.0.1";
		int port = 5425;						
		unsigned int timeout = 100;				

		// Configurar qué información queremos mandar al PVD (debug, profile, memory)
		PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

		// Intentar establecer la conexión
		_pvdConnection = PxVisualDebuggerExt::createConnection(pvdConnectionManager, ip, port, timeout, connectionFlags);

	#endif
	} 

	//________________________________________________________________________

	CServer::~CServer() {
		// Destruir objetos en orden inverso a como fueron creados
		if (_pvdConnection) {
			_pvdConnection->release();
			_pvdConnection = NULL;
		}

		// Antes de liberar los punteros de PhysX, liberamos
		// los material reservados (las geometrias en principio
		// no manejan punteros).
		Physics::CGeometryFactory::Release();
		Physics::CMaterialManager::Release();

		if (_cooking) {
			_cooking->release();
			_cooking = NULL;
		}

		if (_controllerManager) {
			_controllerManager->release();
			_controllerManager = NULL;
		}

		if (_cudaContextManager) {
			_cudaContextManager->release();
			_cudaContextManager = NULL;
		}

		if (_physics) {
			_physics->release();
			_physics = NULL;
		}

		if (_profileZoneManager) { 
			_profileZoneManager->release();
			_profileZoneManager = NULL;
		}

		if (_foundation) {
			_foundation->release();
			_foundation = NULL;
		}
	
		if (_collisionManager) {
			delete _collisionManager;
			_collisionManager = NULL;
		}

		if (_allocator) {
			delete _allocator;
			_allocator = NULL;
		}

		if (_errorManager) {
			delete _errorManager;
			_errorManager = NULL;
		}
	} 

	//________________________________________________________________________

	bool CServer::Init() 
	{
		if (!_instance) {
			_instance = new CServer();
		}

		if (!Physics::CGeometryFactory::Init())
			return false;

		if (!Physics::CMaterialManager::Init())
			return false;

		return true;
	} 

	//________________________________________________________________________

	void CServer::Release() {
		if(_instance) {
			delete _instance;
			_instance = NULL;
		}
	} 

	//________________________________________________________________________

	void CServer::destroyActor(physx::PxActor *actor) {
		assert(_scene);

		// Eliminar el actor de la escena
		_scene->removeActor(*actor);

		// Liberar recursos
		actor->release();
	}

	//________________________________________________________________________

	void CServer::destroyAggregate(physx::PxAggregate* aggregate) {
		assert(_scene);

		/*unsigned int nbActors = aggregate->getNbActors();
		PxActor** actorsBuffer = new PxActor* [nbActors];

		aggregate->getActors(actorsBuffer, nbActors);
		for(unsigned int i = 0; i < nbActors; ++i) {
			Physics::CServer::getSingletonPtr()->destroyActor(actorsBuffer[i]);
		}

		// Liberamos la memoria temporal reservada
		delete actorsBuffer;*/

		// Eliminar agregado de la escena
		_scene->removeAggregate(*aggregate);

		// Liberar recursos
		aggregate->release();
	}

	//________________________________________________________________________

	bool CServer::tick(unsigned int msecs) {
		assert(_scene);
		_acumTime += msecs;

		// Realizamos varias simulaciones en función del timestep (fixedTime) fijado.
		while(_acumTime >= _fixedTime){
			// Simulamos un timestep
			// La llamada a simulate provoca que PhysX mueva a los actores físicos durante
			// el tiempo especificado.
			_scene->simulate(_fixedTime * 0.001f);
			_acumTime -= _fixedTime;
			// Si aún tenemos que hacer más simulaciones, le pedimos a PhysX que nos devuelva
			// los resultados.
			if(_acumTime >= _fixedTime) {
				// fetchResults provoca que se disparen las llamadas a los callbacks como
				// onTrigger y onContact. La variable booleana indica que queremos que esta
				// llamada sea bloqueante.
				_scene->fetchResults(true);
			}
		}
		return _scene->fetchResults(true);
	}

	//________________________________________________________________________

	void CServer::createScene() {
		assert(_instance);
	
		// Crear el descriptor de la escena
		PxSceneDesc sceneDesc( _physics->getTolerancesScale() );

		// Establecer la gravedad en el eje Y
		sceneDesc.gravity = PxVec3(0.0f, -9.81f * 7, 0.0f);

		// Establecer el gestor de colisiones
		sceneDesc.simulationEventCallback = _collisionManager;

		// Establecer un gestor de tareas por CPU
		if (!sceneDesc.cpuDispatcher) {
			int mNbThreads = 1; // hilos de ejecución
			_cpuDispatcher  = PxDefaultCpuDispatcherCreate(mNbThreads);
			assert (_cpuDispatcher && "Error en PxDefaultCpuDispatcherCreate");

			sceneDesc.cpuDispatcher = _cpuDispatcher;
		}
		
		// Establecemos el shader que controla las colisiones entre entidades.
		sceneDesc.filterShader = customFilterShader;

		// Intentar establecer un gestor de tareas por GPU 
		// Sólo Windows
	#ifdef PX_WINDOWS
		if (!sceneDesc.gpuDispatcher && _cudaContextManager) {
			sceneDesc.gpuDispatcher = _cudaContextManager->getGpuDispatcher();
		}
	#endif

		// Crear la escena física
		_scene = _physics->createScene(sceneDesc);
		_acumTime = 0;
		_fixedTime = 8;
		assert(_scene && "Error en PxPhysics::createScene");

		// Activamos la notificación de eventos entre entidades kinemáticas.
		_scene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_PAIRS, true);
		_scene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS, true);

		//CCloth* test = new CCloth();
	}

	//________________________________________________________________________

	void CServer::destroyScene() {
		assert(_instance);

		if (_scene) {
			_scene->release();
			_scene = NULL;
		}
	}

	//________________________________________________________________________


	void CServer::setGroupCollisions(int group1, int group2, bool enable) {
		// Activar / desactivar colisiones entre grupos
		PxSetGroupCollisionFlag(group1, group2, enable);
	}

	//________________________________________________________________________

	/* 
	Por no pertecer a la clase CServer esta función es común a todo el namespace Physics.

	Esta función es la que realmente se encarga que ocurra la magia de las notificaciones.
	Solo seran notificados de colisiones y triggers aquellos actores cuyos grupos de colisión
	hayan sido incluidos en la lista de grupos de colisión del otro actor con el que colisionan.

	Las máscaras y grupos de colisión se fijan en la función setupFiltering.
	*/
	PxFilterFlags customFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
									 PxFilterObjectAttributes attributes1, PxFilterData filterData1,
									 PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize) {
        
		// Si alguno de los dos actores es un trigger...
		if( PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1) ) {
			if( (filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1) )
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		
			return PxFilterFlag::eDEFAULT;
		}

		// Generamos el comportamiento por defecto de contacto entre shapes
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		// Disparar la llamada a onContact de collisonManager siempre que los grupos de colision
		// sean los adecuados
		if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

		return PxFilterFlag::eDEFAULT;
	}

	//________________________________________________________________________

	void CServer::setupFiltering(PxRigidActor* actor, int group, const std::vector<int>& groupList) {
		// El grupo de colision equivale al numero de desplazamientos que podemos realizar,
		// que en nuestro caso son 32 debido a que tenemos un entero de 32 bits.
		PxU32 filterGroup = (1 << group);
		// Si no se ha especificado una lista de grupos asumimos que este elemento no quiere
		// que nadie sea notificado de colisiones o triggers (por lo que asignamos la mascara
		// todo a ceros). En caso contrario, si el primer elemento es un -1 asumimos que 
		// queremos ser notificados con cualquier cosa (el caso del player); si el primer
		// elemento no es -1, asignamos ese valor a la mascara y la incrementamos en el 
		// siguiente paso.
		PxU32 filterMask = groupList.empty() ? 0 : ( groupList[0] == -1 ? 0xFFFFFFFF : (1 << groupList[0]) );
	
		// Calculamos la mascara de colision, es decir, la mascara que define con que grupos
		// de colision debemos activarnos.
		// Para conseguir esto realizamos un OR logico de todos los grupos de colision con los
		// que debemos ser activados.
		// Sabremos si debemos comprobar colisiones con un objeto si al chocar contra nosotros
		// devuelve true al realizar un AND logico con nuestra mascara y su grupo (lo cual
		// querria decir que su grupo esta incluido en la lista de actores a tener en cuenta).
		for(unsigned int i = 1; i < groupList.size(); ++i) {
			filterMask |= (1 << groupList[i]);
		}

		PxFilterData filterData;
		filterData.word0 = filterGroup; // grupo de colision
		filterData.word1 = filterMask;  // mascara de colision - de ella depende que se ejecute onTrigger y onContact
		const PxU32 numShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape* [numShapes];
		actor->getShapes(shapes, numShapes);

		// Asignamos el filtro a todos los shapes de nuestro actor
		for(PxU32 i = 0; i < numShapes; ++i) {
			shapes[i]->setSimulationFilterData(filterData);
			shapes[i]->setQueryFilterData(filterData);
		}

		delete [] shapes;
	}

	//________________________________________________________________________
	
	bool raycastComparator(const CRaycastHit& hit1, const CRaycastHit& hit2) { 
		return hit1.distance < hit2.distance; 
	}

	//________________________________________________________________________

	void CServer::raycastMultiple(const Ray& ray, float maxDistance, std::vector<CRaycastHit>& hits, bool sortResultingArray,
								  unsigned int filterMask) const {

		// Establecer parámettros del rayo
		PxVec3 origin = Vector3ToPxVec3( ray.getOrigin() );      // origen
		PxVec3 unitDir = Vector3ToPxVec3( ray.getDirection() );  // dirección normalizada

		// Seteamos los flags que indican que información queremos extraer
		const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;

		// Variable que indicara si existe un elemento bloqueante
		bool blockingHit;

		// Reservamos memoria para el buffer de colisiones
		PxU32 bufferSize = 256;
		PxRaycastHit* hitBuffer = new(std::nothrow) PxRaycastHit [bufferSize];
		assert(hitBuffer != NULL && "Error: Fallo en la reserva de memoria");

		unsigned int nbHits;
		// Si nos pasan como máscara la 0, hacemos una query normal. En caso contrario
		// hacemos la query usando el filtro que nos dan.
		if(filterMask == 0) {
			nbHits = _scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hitBuffer, bufferSize, blockingHit);
		}
		else {
			PxSceneQueryFilterData filters;
			filters.data.word0 = filterMask;
			nbHits = _scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hitBuffer, bufferSize, blockingHit, filters);
		}

		while(nbHits == -1) {
			// Si el buffer se ha desbordado aumentamos su tamaño al doble
			// y volvemos ha realizar la query
			delete [] hitBuffer;
		
			bufferSize *= 2;
			hitBuffer = new(std::nothrow) PxRaycastHit [bufferSize];
			assert(hitBuffer != NULL && "Error en la reserva de memoria");

			// Realizamos de nuevo la query
			nbHits = _scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hitBuffer, bufferSize, blockingHit);
		}

		if(nbHits > 0) {
			// Si hemos golpeado a otras entidades creamos un buffer
			hits.reserve(nbHits);

			// Rellenamos el buffer con un puntero a cada una de las entidades golpeadas
			IPhysics* component;
			CRaycastHit raycastHit;
			for(int i = 0; i < nbHits; ++i) {
				component = static_cast<IPhysics*>( hitBuffer[i].shape->getActor().userData );

				raycastHit.entity = component->getEntity();
				raycastHit.distance = hitBuffer[i].distance;
				raycastHit.impact = PxVec3ToVector3(hitBuffer[i].impact);
				raycastHit.normal = PxVec3ToVector3(hitBuffer[i].normal);

				hits.push_back(raycastHit);
			}
		}

		delete [] hitBuffer;

		if( !hits.empty() && sortResultingArray ) {
			// Ordenamos el vector de resultados
			std::sort(hits.begin(), hits.end(), raycastComparator);
		}
	}

	//________________________________________________________________________

	bool sweepComparator(const CSweepHit& hit1, const CSweepHit& hit2) { 
		return hit1.distance < hit2.distance; 
	}
	//________________________________________________________________________

	bool CServer::raycastSingle(const Ray& ray, float maxDistance, CRaycastHit& hit, unsigned int filterMask) const {
		// Establecer parámettros del rayo
		PxVec3 origin = Vector3ToPxVec3( ray.getOrigin() );      // origen     
		PxVec3 unitDir = Vector3ToPxVec3( ray.getDirection() );  // dirección normalizada

		// Seteamos los flags que indican que información queremos extraer
		const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;

		// Punto de golpeo del raycast
		PxRaycastHit hitSpot;
		bool validEntity;

		// Si nos pasan como máscara la 0, hacemos una query normal. En caso contrario
		// hacemos la query usando el filtro que nos dan.
		if(filterMask == 0) {
			validEntity = _scene->raycastSingle(origin, unitDir, maxDistance, outputFlags, hitSpot);
		}
		else {
			PxSceneQueryFilterData filters;
			filters.data.word0 = filterMask;
			validEntity = _scene->raycastSingle(origin, unitDir, maxDistance, outputFlags, hitSpot, filters);
		}

		// Introducimos la información devuelta en la estructura que vamos a devolver
		if(validEntity){
			hit.entity		= static_cast<IPhysics*>( hitSpot.shape->getActor().userData )->getEntity();
			hit.distance	= hitSpot.distance;
			hit.impact		= PxVec3ToVector3( hitSpot.impact );
			hit.normal		= PxVec3ToVector3( hitSpot.normal );
		}

		return validEntity;
	}

	//________________________________________________________________________

	bool CServer::raycastAny(const Ray& ray, float maxDistance, unsigned int filterMask) const {
		// Establecer parámettros del rayo
		PxVec3 origin = Vector3ToPxVec3( ray.getOrigin() );      // origen     
		PxVec3 unitDir = Vector3ToPxVec3( ray.getDirection() );  // dirección normalizada
		// Variable de query usada por physx
		PxSceneQueryHit hit;

		if(filterMask == 0) {
			return _scene->raycastAny(origin, unitDir, maxDistance, hit);
		}
		else {
			PxSceneQueryFilterData filters;
			filters.data.word0 = filterMask;
			return _scene->raycastAny(origin, unitDir, maxDistance, hit, filters);
		}
	}

	//________________________________________________________________________

	void CServer::sweepMultiple(const physx::PxGeometry& geometry, const Vector3& position,
								const Vector3& unitDir, float distance, std::vector<CSweepHit>& hitSpots, 
								bool sortResultingArray, unsigned int filterMask) const {

		// Booleano que indicara si hay elementos que bloquean el hit
		bool blockingHit;
		// La situamos en la posicion dada
		PxTransform pose( Vector3ToPxVec3(position) );
		// Seteamos el tamaño del buffer de colisiones a 5
		PxU32 bufferSize = 5;
		// Reservamos memoria para el buffer
		PxSweepHit* hitBuffer = new(std::nothrow) PxSweepHit [bufferSize];
		assert(hitBuffer != NULL && "Error en la reserva de memoria");

		// Seteamos los flags de sweep
		const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE			| 
											  PxSceneQueryFlag::eIMPACT				| 
											  PxSceneQueryFlag::eNORMAL;

		unsigned int nbHits;
		// Si nos pasan como máscara la 0, hacemos una query normal. En caso contrario
		// hacemos la query usando el filtro que nos dan.
		if(filterMask == 0) {
			nbHits = _scene->sweepMultiple(geometry, pose, Vector3ToPxVec3(unitDir), distance, outputFlags, hitBuffer, bufferSize, blockingHit);
		}
		else {
			PxSceneQueryFilterData filters;
			filters.data.word0 = filterMask;
			nbHits = _scene->sweepMultiple(geometry, pose, Vector3ToPxVec3(unitDir), distance, outputFlags, hitBuffer, bufferSize, blockingHit, filters);
		}

		while(nbHits == -1) {
			// Si el buffer se ha desbordado aumentamos su tamaño al doble
			// y volvemos ha realizar la query
			delete [] hitBuffer;
		
			bufferSize *= 2;
			hitBuffer = new(std::nothrow) PxSweepHit [bufferSize];
			assert(hitBuffer != NULL && "Error en la reserva de memoria");

			// Realizamos de nuevo la query
			nbHits = _scene->sweepMultiple(geometry, pose, Vector3ToPxVec3(unitDir), 100, outputFlags, hitBuffer, bufferSize, blockingHit);
		}

		if(nbHits > 0) {
			// Si hemos golpeado a otras entidades creamos un buffer
			hitSpots.reserve(nbHits);

			IPhysics* component;
			CSweepHit sweepHit;
			// Rellenamos el buffer con un puntero a cada una de las entidades golpeadas
			for(int i = 0; i < nbHits; ++i) {
				component = static_cast<IPhysics*>( hitBuffer[i].shape->getActor().userData );

				sweepHit.distance = hitBuffer[i].distance;
				sweepHit.impact = PxVec3ToVector3(hitBuffer[i].impact);
				sweepHit.normal = PxVec3ToVector3(hitBuffer[i].normal);
				sweepHit.entity = component->getEntity();
				
				hitSpots.push_back(sweepHit);
			}
		}

		delete [] hitBuffer;

		if( !hitSpots.empty() && sortResultingArray ) {
			// Ordenamos el vector de resultados
			std::sort(hitSpots.begin(), hitSpots.end(), sweepComparator);
		}
	}

	//________________________________________________________________________

	bool CServer::sweepSingle(const physx::PxGeometry& sweepGeometry, const Vector3& position, 
						      const Vector3& unitDir, float distance, Vector3& hitSpot, unsigned int filterMask) const {

		// Seteamos los flags de sweep
		const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;

		// Situamos la geometria de sweep en la posicion dada
		PxTransform pose( Vector3ToPxVec3(position) );
		// Contendra el hit obtenido
		PxSweepHit hit;

		bool status;
		if(filterMask == 0) {
			status = _scene->sweepSingle(sweepGeometry, pose, Vector3ToPxVec3(unitDir), distance, outputFlags, hit);
		}
		else {
			PxSceneQueryFilterData filters;
			filters.data.word0 = filterMask;
			status = _scene->sweepSingle(sweepGeometry, pose, Vector3ToPxVec3(unitDir), distance, outputFlags, hit, filters);
		}

		hitSpot = status ? PxVec3ToVector3(hit.impact) : Vector3::ZERO;
		return status;

	}

	//________________________________________________________________________

	bool CServer::sweepAny(const physx::PxGeometry& sweepGeometry, const Vector3& position, 
						   const Vector3& unitDir, float distance, unsigned int filterMask) const {

		// Seteamos los flags de sweep
		const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;

		// Situamos la geometria de sweep en la posicion dada
		PxTransform pose( Vector3ToPxVec3(position) );
		// Contendra el hit obtenido
		PxSweepHit hit;

		if(filterMask == 0) {
			return _scene->sweepAny(sweepGeometry, pose, Vector3ToPxVec3(unitDir), distance, outputFlags, hit);
		}
		else {
			PxSceneQueryFilterData filters;
			filters.data.word0 = filterMask;
			return _scene->sweepAny(sweepGeometry, pose, Vector3ToPxVec3(unitDir), distance, outputFlags, hit, filters);
		}
	}

	//________________________________________________________________________

	void CServer::overlapMultiple(const PxGeometry& geometry, const Vector3& position, std::vector<Logic::CEntity*>& entitiesHit, unsigned int filterMask) const {
		// Comprobar que es una de las geometrias soportadas por la query de overlap

		// La situamos en la posicion dada
		PxTransform pose( Vector3ToPxVec3(position) );
		// Seteamos el tamaño del buffer de colisiones a 5
		PxU32 bufferSize = 5;
		// Reservamos memoria para el buffer
		PxShape** hitBuffer = new(std::nothrow) PxShape* [bufferSize];
		assert(hitBuffer != NULL && "Error en la reserva de memoria");

		// Calculamos el overlap contra objetos dinamicos y contra estaticos.
		// El valor de retorno es el numero de hits del buffer o -1 si el buffer no es lo suficientemente
		// grande.
		PxSceneQueryFilterData filterData( PxSceneQueryFilterFlags(PxSceneQueryFilterFlag::eDYNAMIC | PxSceneQueryFilterFlag::eSTATIC) );

		// Si nos pasan como máscara la 0, hacemos una query normal. En caso contrario
		// hacemos la query usando el filtro que nos dan.
		if(filterMask != 0) {
			filterData.data.word0 = filterMask;
		}

		unsigned int nbHits = _scene->overlapMultiple(geometry, pose, hitBuffer, bufferSize, filterData);
		while(nbHits == -1) {
			// Si el buffer se ha desbordado aumentamos su tamaño al doble
			// y volvemos ha realizar la query
			delete [] hitBuffer;
		
			bufferSize *= 2;
			hitBuffer = new(std::nothrow) PxShape* [bufferSize];
			assert(hitBuffer != NULL && "Error en la reserva de memoria");

			// Realizamos de nuevo la query
			nbHits = _scene->overlapMultiple(geometry, pose, hitBuffer, bufferSize, filterData);
		}

		if(nbHits > 0) {
			// Si hemos golpeado a otras entidades creamos un buffer
			entitiesHit.reserve(nbHits);

			// Rellenamos el buffer con un puntero a cada una de las entidades golpeadas
			for(int i = 0; i < nbHits; ++i) {
				IPhysics *component = static_cast<IPhysics*>( hitBuffer[i]->getActor().userData );
				entitiesHit.push_back( component != NULL ? component->getEntity() : NULL );
			}
		}

		delete [] hitBuffer;
	}

	//________________________________________________________________________

	bool CServer::overlapAny(const PxGeometry& geometry, const Vector3& position, unsigned int filterMask) const {
		PxShape* hit;

		if(filterMask == 0) {
			return _scene->overlapAny(geometry, PxTransform ( Vector3ToPxVec3(position) ), hit);
		}
		else {
			PxSceneQueryFilterData filters;
			filters.data.word0 = filterMask;
			return _scene->overlapAny(geometry, PxTransform ( Vector3ToPxVec3(position) ), hit, filters);
		}
	}

} // namespace Physics