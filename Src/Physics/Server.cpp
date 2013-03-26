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

	bool CServer::tick(unsigned int msecs) {
		assert(_scene);
		_acumTime += msecs;

		// Realizamos varias simulaciones en función del timestep (fixedTime) fijado.
		while(_acumTime >= _fixedTime){
			// Simulamos un timestep
			// La llamada a simulate provoca que PhysX mueva a los actores físicos durante
			// el tiempo especificado.
			_scene->simulate(_fixedTime / 1000.0f);
			_acumTime -= 8;

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
		PxU32 filterMask = groupList.empty() ? 0 : ( groupList[0] == -1 ? 0xFFFF : (1 << groupList[0]) );
	
		// Calculamos la mascara de colision, es decir, la mascara que define con que grupos
		// de colision debemos activarnos.
		// Para conseguir esto realizamos un OR logico de todos los grupos de colision con los
		// que debemos ser activados.
		// Sabremos si debemos comprobar colisiones con un objeto si al chocar contra nosotros
		// devuelve true al realizar un AND logico con nuestra mascara y su grupo (lo cual
		// querria decir que su grupo esta incluido en la lista de actores a tener en cuenta).
		for(int i = 1; i < groupList.size(); ++i) {
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
		}

		delete [] shapes;
	}

	//________________________________________________________________________

	Logic::CEntity* CServer::raycastClosest (const Ray& ray, float maxDist) const {
		assert(_scene);

		// Establecer parámettros del rayo
		PxVec3 origin = Vector3ToPxVec3( ray.getOrigin() );		// origen     
		PxVec3 unitDir = Vector3ToPxVec3( ray.getDirection() ); // dirección normalizada   
		PxReal maxDistance = maxDist;							// distancia máxima
		PxRaycastHit hit;                 

		// Información que queremos recuperar de la posible colisión
		//const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;
		const PxSceneQueryFlags outputFlags;

		// Lanzar el rayo
		bool intersection = _scene->raycastSingle(origin, unitDir, maxDistance, outputFlags, hit);
	
		// IMPORTANTE: aunque se haya llamado al método move de los controllers y al consultar su posición
		// esta aparezca actualizada, sus actores asociados no se habrán desplazado aún. La consecuencia
		// es que se pueden recuperar colisiones inesperadas.

		if (intersection) {
			// Devolver entidad lógica asociada a la entidad física impactada
			IPhysics* component = static_cast<IPhysics*>( hit.shape->getActor().userData );
			return component->getEntity();
		} 
		else {
			return NULL;
		}
	}

	//________________________________________________________________________

	Logic::CEntity* CServer::raycastClosest(const Ray& ray, float maxDist, int group) const {
		assert(_scene);

		// Establecer parámettros del rayo
		PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());      // origen     
		PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());  // dirección normalizada   
		PxReal maxDistance = maxDist;                          // distancia máxima
		PxRaycastHit hit;                 
		const PxSceneQueryFlags outputFlags;				   // Info que queremos recuperar	

		// Lanzar el rayo
		PxRaycastHit hits[64];
		bool blockingHit;
		PxI32 nHits = _scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hits, 64, blockingHit); 

		// Buscar un actot que pertenezca al grupo de colisión indicado
		for (int i = 0; i < nHits; ++i) {
			PxRigidActor* actor = &hits[i].shape->getActor();
			if ( PxGetGroup(*actor) == group ) {
				IPhysics* component = static_cast<IPhysics*>(actor->userData);
				if (component) {
					return component->getEntity();
				}
			}
		}

		return NULL;

		// Nota: seguro que se puede hacer de manera mucho más eficiente usando los filtros
		// de PhysX.
	}

	//________________________________________________________________________

	Logic::CEntity* CServer::raycastClosestInverse(const Ray& ray, float maxDist, unsigned int id) const {
		assert(_scene);

		// Establecer parámettros del rayo
		PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());      // origen     
		PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());  // dirección normalizada   
		PxReal maxDistance = maxDist;                          // distancia máxima
		PxRaycastHit hit;                 
		const PxSceneQueryFlags outputFlags;				   // Info que queremos recuperar	

		// Lanzar el rayo
		PxRaycastHit hits[60];
		bool blockingHit;

		PxI32 nHits = _scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hits, 60, blockingHit); 
	
		// Buscar un actot que pertenezca al grupo de colisión indicado
		for (int i = nHits - 1; i >= 0; --i) {
			PxRigidActor* actor = &hits[i].shape->getActor();
			PxShapeFlags* flags = &hits[i].shape->getFlags();
			IPhysics *component = static_cast<IPhysics*>(actor->userData);

			if(component != NULL) {
				Logic::CEntity* entityHit = component->getEntity();

				if(entityHit->getEntityID() != id && !(*flags & PxShapeFlag::eTRIGGER_SHAPE)) {
					return entityHit;
				}
			}
		}

		return NULL;

		// Nota: seguro que se puede hacer de manera mucho más eficiente usando los filtros
		// de PhysX.
	}

	//________________________________________________________________________

	void CServer::overlapMultiple(const PxGeometry& geometry, const Vector3& position, Logic::CEntity** & entitiesHit, int& nbHits) {
		// Comprobar que es una de las geometrias soportadas por la query de overlap

		// La situamos en la posicion dada
		PxTransform pose( Vector3ToPxVec3(position) );
		// Seteamos el tamaño del buffer de colisiones a 5
		PxU32 bufferSize = 5;
		// Reservamos memoria para el buffer
		PxShape** hitBuffer = new(std::nothrow) PxShape* [bufferSize];
		assert(hitBuffer != NULL && "Error en la reserva de memoria");

		// Calculamos el overlap contra objetos dinamicos (ya que los estaticos no nos interesan).
		// El valor de retorno es el numero de hits del buffer o -1 si el buffer no es lo suficientemente
		// grande.
		PxSceneQueryFilterData filterData(PxSceneQueryFilterFlag::eDYNAMIC);

		nbHits = _scene->overlapMultiple(geometry, pose, hitBuffer, bufferSize, filterData);
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
			entitiesHit = new(std::nothrow) Logic::CEntity* [nbHits];
			assert(entitiesHit != NULL && "Error en la reserva de memoria");

			// Rellenamos el buffer con un puntero a cada una de las entidades golpeadas
			for(int i = 0; i < nbHits; ++i) {
				IPhysics *component = static_cast<IPhysics*>( hitBuffer[i]->getActor().userData );
				entitiesHit[i] = component != NULL ? component->getEntity() : NULL;
			}
		}

		delete [] hitBuffer;
	}

	//________________________________________________________________________

	bool CServer::overlapAny(const PxGeometry& geometry, const Vector3& position) {
		PxShape* hit;
		return _scene->overlapAny(geometry, PxTransform ( Vector3ToPxVec3(position) ), hit);
	}

} // namespace Physics