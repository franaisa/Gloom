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
						 _actor(NULL) {
	} // CEntity

	//________________________________________________________________________

	CEntity::CEntity(physx::PxRigidActor* adoptActor) : _isTrigger(false) {
		// Asignamos el actor
		this->_actor = adoptActor;
	}

	//________________________________________________________________________

	CEntity::~CEntity() {
		if(_actor != NULL) {
			// El server se encarga de desatachar la entidad fisica de la
			// escena a la que pertenezca y liberar los recursos reservados
			// por ella
			Physics::CServer::getSingletonPtr()->destroyActor(_actor);
			_actor = NULL;
		}
	} // ~CEntity

	//________________________________________________________________________

	void CEntity::load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component) {
		deserializeFromRepXFile(file, group, groupList, component);
	}

	//________________________________________________________________________

	void CEntity::deserializeFromRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component) {
		// Obtenemos el puntero al servidor de fisicas
		Physics::CServer* physicsServer = Physics::CServer::getSingletonPtr();
		PxScene* scene = physicsServer->getActiveScene();
		PxPhysics* physics = physicsServer->getPhysxSDK();
		PxCooking* cooking = physicsServer->getCooking();
		assert(scene);

		// Preparar parámetros para deserializar
		PxDefaultFileInputData data(file.c_str());
		PxCollection* bufferCollection = physics->createCollection();
		PxCollection* sceneCollection = physics->createCollection(); 
		PxStringTable* stringTable = NULL; 
		PxUserReferences* externalRefs = NULL; 
		PxUserReferences* userRefs = NULL; 

		// Deserializar a partir del fichero RepX
		repx::deserializeFromRepX(data, *physics, *cooking, stringTable, externalRefs, 
								  *bufferCollection, *sceneCollection, userRefs);


		// Añadir entidades físicas a la escena
		physics->addCollection(*sceneCollection, *scene);

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

		// Liberar recursos
		bufferCollection->release();
		sceneCollection->release();
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

	void CEntity::deactivateSimulation() {
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

	std::string CEntity::getName() {
		return _actor->getName();
	}

	//________________________________________________________________________

	bool CEntity::isDynamic() {
		return _actor->isRigidDynamic();
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
