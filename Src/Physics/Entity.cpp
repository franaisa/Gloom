//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que encapsula las operaciones 
básicas de los rigid bodies.

@see Physics::CEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "Entity.h"
#include "Conversions.h"
#include "Physics/Server.h"
#include "Physics/CollisionManager.h"
#include "Logic/Entity/Components/Physics.h"

#include <assert.h>

#include <PxPhysics.h>
#include <PxScene.h>
#include <PxShape.h>
#include <PxRigidActor.h>
#include <PxAggregate.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include <PxStringTable.h>
#include <cooking/PxCooking.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxSimpleFactory.h>
#include <extensions/PxDefaultStreams.h>
#include <extensions/PxStringTableExt.h>
#include <geometry/PxGeometryHelpers.h>
#include <RepX/RepXUtility.h>

using namespace physx;
using namespace std;

namespace Physics {

	CEntity::CEntity() : _isTrigger(false),
						 _actor(NULL),
						 _aggregate(NULL) {

		Physics::CServer* physicsServer = Physics::CServer::getSingletonPtr();

		// Obtenemos la sdk de physics y comprobamos que ha sido inicializada
		_physxSDK = physicsServer->getPhysxSDK();
		assert(_physxSDK && "No se ha inicializado physX");
		
		// Obtenemos la clase de cocinado de physX y comprobamos que ha sido inicializada
		_cooking = physicsServer->getCooking();
		assert(_cooking && "El cocinado de physx no ha sido activado");

		// Obtenemos la escena de physx y comprobamos que ha sido inicializada
		_scene = physicsServer->getActiveScene();
		assert(_scene && "No existe una escena fisica");

		// Obtenemos el gestor de colisiones
		_collisionManager = physicsServer->getCollisionManager();
		assert(_collisionManager && "No existe ningun gestor de colisiones");
	} // CEntity

	//________________________________________________________________________

	CEntity::~CEntity() {
		if(_actor != NULL) {
			// El server se encarga de desatachar la entidad fisica de la
			// escena a la que pertenezca y liberar los recursos reservados
			// por ella
			Physics::CServer::getSingletonPtr()->destroyActor(_actor);
			_actor = NULL;
		}

		if(_aggregate != NULL) {
			Physics::CServer::getSingletonPtr()->destroyAggregate(_aggregate);
			_aggregate = NULL;
		}

		// Fijamos los punteros a physx como nulos
		_physxSDK = NULL;
		_cooking = NULL;
		_scene = NULL;
		_collisionManager = NULL;
	} // ~CEntity

	//________________________________________________________________________

	void CEntity::load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component, bool nameActors) {
		deserializeFromRepXFile(file, group, groupList, component, nameActors);
	}

	//________________________________________________________________________

	vector<string> CEntity::getActorNames() {
		vector<string> actorNamesList;

		unsigned int nbActors = _aggregate->getNbActors();
		PxActor** actorsBuffer = new PxActor* [nbActors];

		actorNamesList.reserve(nbActors);

		_aggregate->getActors(actorsBuffer, nbActors);
		for(unsigned int i = 0; i < nbActors; ++i) {
			actorNamesList.push_back( actorsBuffer[i]->getName() );
		}

		// Liberamos la memoria temporal reservada
		delete actorsBuffer;

		return actorNamesList;
	}
	
	//________________________________________________________________________

	std::vector< physx::PxActor* > CEntity::getActors() {
		std::vector< physx::PxActor* > actors;

		unsigned int nbActors = _aggregate->getNbActors();
		PxActor** actorsBuffer = new PxActor* [nbActors];

		actors.reserve(nbActors);

		_aggregate->getActors(actorsBuffer, nbActors);
		for(unsigned int i = 0; i < nbActors; ++i) {
			actors.push_back( actorsBuffer[i] );
		}

		// Liberamos la memoria temporal reservada
		delete actorsBuffer;

		return actors;
	}

	//________________________________________________________________________

	void CEntity::deserializeFromRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component,
										  bool nameActors) {
		assert(_scene);

		// Preparar parámetros para deserializar
		PxDefaultFileInputData data(file.c_str());
		PxCollection* bufferCollection = _physxSDK->createCollection();
		PxCollection* sceneCollection = _physxSDK->createCollection();
		//PxStringTable* stringTable = nameActors ? &PxStringTableExt::createStringTable( CServer::getSingletonPtr()->getFoundation()->getAllocatorCallback() ) : NULL; 
		PxStringTable* stringTable = &PxStringTableExt::createStringTable( CServer::getSingletonPtr()->getFoundation()->getAllocatorCallback() ); 
		PxUserReferences* externalRefs = NULL; 
		PxUserReferences* userRefs = NULL; 

		// Deserializar a partir del fichero RepX
		repx::deserializeFromRepX(data, *_physxSDK, *_cooking, stringTable, externalRefs, 
								  *bufferCollection, *sceneCollection, userRefs);

		// Obtenemos el número de actores que debemos cargar
		PxU32 nbActors = sceneCollection->getNbObjects();

		if(nbActors > 1) {
			deserializeAggregate(sceneCollection, nbActors, component, group, groupList);
		}
		else {
			deserializeActor(sceneCollection, component, group, groupList);
		}

		// Liberar recursos
		bufferCollection->release();
		sceneCollection->release();
	}

	//________________________________________________________________________

	void CEntity::deserializeAggregate(PxCollection* sceneCollection, unsigned int nbActors, const Logic::IPhysics* component, int group, const std::vector<int>& groupList) {
		// Creamos un agregado con el numero maximo de actores leidos y activamos
		// las colisiones para el propio ragdoll (selfcollisions = true)
		_aggregate = _physxSDK->createAggregate(nbActors, true);

		// Asumimos que los datos contenidos en el fichero corresponden a los colliders
		// y articulaciones de un ragdoll
		PxSerializable* serializable;
		PxActor* actor = NULL;
		PxRigidActor* rigid = NULL;
		for (unsigned int i = 0; i < nbActors; ++i) {
			serializable = sceneCollection->getObject(i);
			
			// Las articulaciones también son actores
			if( actor = serializable->is<PxActor>() ) {
				// Añadimos el actor al conjunto de actores del ragdoll
				_aggregate->addActor(*actor);
				// Anotamos el componente lógico asociado a la entidad física
				actor->userData = (void*)component;

				// Si es un collider y no una articulación (hitbox) le asignamos
				// los filtros de colisión que correspondan
				if( rigid = serializable->is<PxRigidActor>() ) {
					// Establecemos el grupo de colision
					PxSetGroup(*rigid, group);
					// Establecemos los filtros de colisión
					Physics::CServer::getSingletonPtr()->setupFiltering(rigid, group, groupList);

					// Para pasar los shapes a kinematicos -------------------------- (testing)
					PxRigidDynamic* dyn = serializable->is<PxRigidDynamic>();
					if(dyn)
						dyn->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, true);
					
				}
			}
		}

		// Añadimos el agregado a la escena
		_scene->addAggregate(*_aggregate);
	}

	//________________________________________________________________________

	void CEntity::deserializeActor(PxCollection* sceneCollection, const Logic::IPhysics* component, int group, const std::vector<int>& groupList) {
		// Añadir entidades físicas a la escena
		_physxSDK->addCollection(*sceneCollection, *_scene);

		// Buscar una entidad de tipo PxRigidActor. Asumimos que hay exactamente 1 en el fichero.
		_actor = NULL;
		for (unsigned int i = 0; i < sceneCollection->getNbObjects() && !_actor; ++i) {
			PxSerializable* p = sceneCollection->getObject(i);
			_actor = p->is<PxRigidActor>();
		}
		assert(_actor);

		// Anotar el componente lógico asociado a la entidad física
		_actor->userData = (void*)component;

		// Establecer el grupo de colisión
		PxSetGroup(*_actor, group);
		// Establecer los filtros de colisión
		Physics::CServer::getSingletonPtr()->setupFiltering(_actor, group, groupList);
	}

	//________________________________________________________________________

	Matrix4 CEntity::getTransform() const {
		return PxTransformToMatrix4( _actor->getGlobalPose() );
	}

	//________________________________________________________________________

	Vector3 CEntity::getPosition() const {
		return PxVec3ToVector3( _actor->getGlobalPose().p );
	}

	//________________________________________________________________________

	Quaternion CEntity::getOrientation() const {
		return PxQuatToQuaternion( _actor->getGlobalPose().q );
	}

	//________________________________________________________________________

	void CEntity::activateSimulation() {
		if(_actor != NULL) {
			activateSimulation(_actor);
		}

		if(_aggregate != NULL) {
			unsigned int nbActors = _aggregate->getNbActors();
			PxActor** actorsBuffer = new PxActor* [nbActors];

			_aggregate->getActors(actorsBuffer, nbActors);
			for(unsigned int i = 0; i < nbActors; ++i) {
				activateSimulation(actorsBuffer[i]);
			}

			// Liberamos la memoria temporal reservada
			delete actorsBuffer;
		}
	}

	//________________________________________________________________________

	void CEntity::deactivateSimulation() {
		if(_actor != NULL) {
			deactivateSimulation(_actor);
		}

		if(_aggregate != NULL) {
			unsigned int nbActors = _aggregate->getNbActors();
			PxActor** actorsBuffer = new PxActor* [nbActors];

			_aggregate->getActors(actorsBuffer, nbActors);
			for(unsigned int i = 0; i < nbActors; ++i) {
				deactivateSimulation(actorsBuffer[i]);
			}

			// Liberamos la memoria temporal reservada
			delete actorsBuffer;
		}
	}

	//________________________________________________________________________

	void CEntity::activateSimulation(physx::PxActor* actor) {
		// Activamos todos los shapes del componente por completo en PhysX
		// Para ello, obtenemos todos sus shapes y ponemos los flags a true

		int nbShapes = _actor->getNbShapes();
		PxShape** actorShapes = new PxShape* [nbShapes];
		_actor->getShapes(actorShapes, nbShapes);
		for(int i = 0; i < nbShapes; ++i) {
			if(_isTrigger) {
				// Volvemos a activar la shape como trigger
				actorShapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
			}

			// Esta shape tomara parte en barridos, raycasts...
			actorShapes[i]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
			// Esta shape entrara dentro de la simulacion de fisicas
			actorShapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE , true);
		}

		delete [] actorShapes;
	}

	//________________________________________________________________________

	void CEntity::deactivateSimulation(physx::PxActor* actor) {
		// Desactivamos todos los shapes del componente por completo en PhysX
		// Para ello, obtenemos todos sus shapes y ponemos los flags a false

		int nbShapes = _actor->getNbShapes();
		PxShape** actorShapes = new PxShape* [nbShapes];
		_actor->getShapes(actorShapes, nbShapes);
		for(int i = 0; i < nbShapes; ++i) {
			if(_isTrigger) {
				// Desactivamos la shape como trigger
				actorShapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
			}

			// Esta shape no tomara parte en barridos, raycasts...
			actorShapes[i]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
			// Esta shape no entrara dentro de la simulacion de fisicas
			actorShapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE , false);
		}

		delete [] actorShapes;
	}

	//________________________________________________________________________

	float CEntity::getLogicPivotOffset(const PxGeometry& geometry) {
		switch( geometry.getType() ) {
			case PxGeometryType::eBOX:
				// Devolver la altura media de la caja
				return static_cast<const PxBoxGeometry*>(&geometry)->halfExtents.y;
				break;

			case PxGeometryType::eSPHERE:
				// Devolver el radio de la esfera
				return static_cast<const PxSphereGeometry*>(&geometry)->radius;
				break;

			case PxGeometryType::eCAPSULE:
				// Devolver el radio de la cupula mas la mitad de la altura
				return static_cast<const PxCapsuleGeometry*>(&geometry)->halfHeight;
				break;
		}

		return 0;
	}

} // namespace Physics
