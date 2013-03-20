//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

@see Physics::CEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "Entity.h"

#include <assert.h>

#include "Physics/Server.h"
#include "Physics/CollisionManager.h"
#include "Conversions.h"

#include "Logic/Entity/Components/Physics.h"

#include <PxPhysics.h>
#include <PxScene.h>
#include <PxShape.h>
#include <PxRigidActor.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxSimpleFactory.h>
#include <geometry/PxGeometryHelpers.h>

using namespace physx;

namespace Physics {

	CEntity::CEntity() : _isTrigger(false) {
		// Obtenemos la sdk de physics y comprobamos que ha sido inicializada
		_physxSDK = Physics::CServer::getSingletonPtr()->getPhysxSDK();
		assert(_physxSDK && "No se ha inicializado physX");
		
		// Obtenemos la escena de physx y comprobamos que ha sido inicializada
		_scene = Physics::CServer::getSingletonPtr()->getActiveScene();
		assert(_scene && "No existe una escena fisica");

		_collisionManager = Physics::CServer::getSingletonPtr()->getCollisionManager();
	} // CEntity

	//________________________________________________________________________

	CEntity::~CEntity() {
		// Destruimos el actor de physx asociado al controller y desligamos el 
		// actor de la escena
		if(_actor != NULL) {
			_actor->release();
			_actor = NULL;
		}

		// Fijamos los punteros a physx como nulos
		_physxSDK = NULL;
		_scene = NULL;
		_collisionManager = NULL;
	} // ~CEntity

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

	//________________________________________________________________________

	PxRigidActor* CEntity::deserializeRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component) {
		return Physics::CServer::getSingletonPtr()->createFromFile(file, group, groupList, component);
	}

	//________________________________________________________________________

	Matrix4 CEntity::getTransform() const {
		return PxTransformToMatrix4( _actor->getGlobalPose() );
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

} // namespace Physics
