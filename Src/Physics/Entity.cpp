//---------------------------------------------------------------------------
// Fluid.cpp
//---------------------------------------------------------------------------

/**
@file Fluid.cpp

@see Graphics::CFluid

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "Fluid.h"
//#include "Scene.h"

#include <assert.h>

#include <Physics/Server.h>
#include <PxPhysicsAPI.h> // Cambiarlo!! Solo incluir lo necesario

using namespace physx;

namespace Physics {

	CFluid::CFluid() : _runOnGPU(false) {

	} // CEntity

	//________________________________________________________________________

	CFluid::~CFluid()
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		
	} // ~CEntity
	
	//________________________________________________________________________
		
	/*bool CFluid::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡La entidad debe asociarse a una escena!!");
		// Si la entidad está cargada por otro gestor de escena.
		if(_loaded && (_scene != scene))
			return false;

		// Si no está cargada forzamos su carga.
		if (!_loaded)
		{
			_scene = scene;
			return load();
		}

		// Si ya estaba cargada en la escena se devuelve cierto.
		return true;

	}*/ // attachToScene
	
	//________________________________________________________________________
		
	bool CFluid::deattachFromScene()
	{
		// Si la entidad no está cargada no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!_loaded)
			return false;
		// Si la entidad está cargada forzamos su descarga.
		else
		{
			assert(_scene && "¡¡La entidad debe estar asociada a una escena!!");
			unload();
			_scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//________________________________________________________________________
		
	void CFluid::unload()
	{
		/*if(_entityNode)
		{
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}
		if(_entity)
		{
			_scene->getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
		}
		*/
	} // load

	//________________________________________________________________________
		
	void CFluid::tick(float secs) {
		//_text->update();
	} // tick

	//________________________________________________________________________

	/*
	=======================================================================================
	** Elementos que tienen que ser especificados en la creacion del sistema de particulas:
	=======================================================================================
	- maxParticles: Maximo numero de particulas que se pueden añadir al sistema. Cuanto menor
	menos memoria se consume
	- particleBaseFlags, PxParticleBaseFlag::ePER_PARTICLE_REST_OFFSET: Activa/desactiva per
	particle rest offset. Cuando esta desactivado se consume menos memoria.


	=============================================================================================
	** Elementos que no pueden variar una vez que el sistema de particulas se atacha a la escena:
	=============================================================================================
	- maxMotionDistance: Máxima distancia a la que una particula puede viajar durante un step
	de simulación. Valores altos pueden afectar al rendimiento, mientras que valores pequeños
	pueden hacer que las particulas apenas se muevan.
	- gridSize: Una pista para que la sdk de physx sepa como elegir el tipo de agrupacion
	para temas de paralelizacion y demás.
	- restOffset: Define la minima distancia entre particulas y actores rigidos.
	- contactOffset: Define la distancia a la que se crea contacto entre las particulas y los
	rigid actors. TIENE QUE SER MAYOR QUE RESTOFFSET.
	- particleReadDataFlags: Especifica un subconjunto de propiedades de simulacion que se
	retornan a la aplicacion tras la simulacion.
	- particleBaseFlags, PxParticleBaseFlag::eGPU: Activa/Desactiva la aceleracion por gpu.
	- particleBaseFlags, PxParticleBaseFlag::eCOLLISION_TWOWAY: Activa/desactiva la interacion
	mutua entre rigidbodies y particulas.


	==========================================================================================
	** Elementos que no pueden variar una vez que el sistema de FLUIDOS se atacha a la escena:
	==========================================================================================
	- restParticleDistance: Resolucion del sistema de fluidos.


	===========================================
	** Propiedades variables de PxParticleBase:
	===========================================
	- restitution: Restitucion del sistema de particulas en la colision.
	- dynamicFriction: Friccion dinamica.
	- staticFriction: Friccion estatica.
	- damping: Velocidad constante de damping, la cual se aplica a todas las particulas.
	- externalAcceleration: Aceleracion que se aplica a cada particula en cada step. La
	gravedad se puede desactivar usando PxActorFlag::eDISABLE_GRAVITY.
	- particleBaseFlags, PxParticleBaseFlag::eENABLED: Activa/Desactiva la simulacion de
	particulas.
	- particleBaseFlags, PxParticleBaseFlag::ePROJECT_TO_PLANE: Activa/desactiva el modo
	de proyeccion que mantiene a las particulas en un plano
	- projectionPlaneNormal, projectionPlaneDistance: Define el plano para el modelo de proyeccion.
	- particleMass: Mass utilizada para interaccion twoway.
	- simulationFilterData: filtro para filtrar colisiones entre particulas y rigid bodies.


	===========================================
	** Propiedades mutables de PxParticleFluid:
	===========================================
	- stiffness: Define la constante de gas. Valores bajos hacen que el fluido sea mas
	compresible, mientras que los valores altos lo hacen mas incompresible (?¿). Los valores
	altos hacen que el sistema sea inestable. Hay que mantener siempre valores entre 1 y 200.
	- viscosity: Controla la densidad del fluido. Cuanta mas viscosidad, mas pesado y lento, cuanta
	menos, mas se parece al agua. Valores razonables estan entre 5 y 300.
	*/

	PxParticleFluid* CFluid::createFluid(unsigned int maxParticles, float restitution, float viscosity,
								         float stiffness, float dynamicFriction, float particleDistance) {	

		_fluid = _physics->createParticleFluid(maxParticles, true);
		assert(_fluid && "PxPhysics::createParticleFluid returned NULL\n");
		_fluid->setGridSize(5.0f);
		_fluid->setMaxMotionDistance(0.3f);
		_fluid->setRestOffset(particleDistance*0.3f);
		_fluid->setContactOffset(particleDistance*0.3f*2);
		_fluid->setDamping(0.0f);
		_fluid->setRestitution(restitution);
		_fluid->setDynamicFriction(dynamicFriction);
		_fluid->setRestParticleDistance(particleDistance);
		_fluid->setViscosity(viscosity);
		_fluid->setStiffness(stiffness);
		_fluid->setParticleReadDataFlag(PxParticleReadDataFlag::eVELOCITY_BUFFER, true);
	
	#if PX_SUPPORT_GPU_PHYSX
		_fluid->setParticleBaseFlag(PxParticleBaseFlag::eGPU, _runOnGPU);
	#endif
		_scene->addActor(*_fluid);
		assert(_fluid->getScene() && "PxScene::addActor failed\n");

	#if PX_SUPPORT_GPU_PHYSX
		//check gpu flags after adding to scene, cpu fallback might have been used.
		_runOnGPU = _runOnGPU && (_fluid->getParticleBaseFlags() & PxParticleBaseFlag::eGPU);
	#endif

		return _fluid;
	}

	//________________________________________________________________________

	bool CFluid::load() {
		// set immutable properties.
		PxU32 maxParticles = 100;
		bool perParticleRestOffset = true;

		// create particle system in PhysX SDK
		PxParticleSystem* ps = _physics->createParticleSystem(maxParticles, perParticleRestOffset);

		// add particle system to scene, in case creation was successful
		if (ps) _scene->addActor(*ps);



		/*
		try
		{
			_entity = _scene->getSceneMgr()->createEntity(_name, _mesh);
			_entity->setCastShadows(true);
		}
		catch(std::exception e)
		{
			return false;
		}
		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_node");
		_entityNode->attachObject(_entity);

		//_text = new CObjectTextDisplay(_entity, _scene->getCamera()->getOgreCamera());
		//_text->enable(false);
		//_text->setText("Gazpacho");
			
		_loaded = true;

		return true;
		*/

		return _loaded = true;
	} // load
	
	//________________________________________________________________________

	/*
	physx::PxRigidStatic* CServer::createStaticSphere(const Vector3 &position, float radius, 
													  bool trigger, int group, const std::vector<int>& groupList, const Logic::IPhysics *component) {

		assert(_scene);

		// Creamos una esfera estática
		PxTransform pose(Vector3ToPxVec3(position));
		PxSphereGeometry geom(radius);
		PxMaterial *material = _defaultMaterial;
		PxTransform localPose(PxVec3(0, radius, 0)); // Transformación de coordenadas lógicas a coodenadas de PhysX
		PxRigidStatic *actor = PxCreateStatic(*_physics, pose, geom, *material, localPose);

		// Transformarlo en trigger si es necesario
		if (trigger) {
			PxShape *shape;
			actor->getShapes(&shape, 1, 0);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		// Anotar el componente lógico asociado a la entidad física
		actor->userData = (void *) component;

		// Establecer el grupo de colisión
		PxSetGroup(*actor, group);

		setupFiltering(actor, group, groupList);

		// Añadir el actor a la escena
		_scene->addActor(*actor);
	
		return actor;
	}

	//________________________________________________________________________

	PxRigidDynamic* CServer::createDynamicSphere(const Vector3 &position, float radius, 
												 float mass, bool kinematic, bool trigger, int group, const std::vector<int>& groupList,  
												 const IPhysics *component) {

		assert(_scene);

		// Creamos una esfera dinámica
		PxTransform pose(Vector3ToPxVec3(position));
		PxSphereGeometry geom(radius);
		PxMaterial *material = _defaultMaterial;
		float density = mass / (4.0/3.0 * 3.141592653589793 * radius * radius * radius);
		PxTransform localPose(PxVec3(0, radius, 0)); // Transformación de coordenadas lógicas a coodenadas de PhysX

		// Crear esfera dinámico o cinemático
		PxRigidDynamic *actor;
		if (kinematic)
			actor = PxCreateKinematic(*_physics, pose, geom, *material, density, localPose);
		else
			actor = PxCreateDynamic(*_physics, pose, geom, *material, density, localPose);
	
		// Transformarlo en trigger si es necesario
		if (trigger) {
			PxShape *shape;
			actor->getShapes(&shape, 1, 0);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		// Anotar el componente lógico asociado a la entidad física
		actor->userData = (void *) component;

		// Establecer el grupo de colisión
		PxSetGroup(*actor, group);

		setupFiltering(actor, group, groupList);

		// Añadir el actor a la escena
		_scene->addActor(*actor);

		return actor;
	}
	*/

} // namespace Physics
