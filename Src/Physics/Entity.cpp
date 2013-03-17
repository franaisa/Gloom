//---------------------------------------------------------------------------
// CEntity.cpp
//---------------------------------------------------------------------------

/**
@file CEntity.cpp

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
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxSimpleFactory.h>

using namespace physx;

namespace Physics {

	CEntity::CEntity() {
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

	void CEntity::loadDynamic(const Vector3 &position, const PxGeometry& geometry, PxMaterial& material,
							  float density, bool kinematic, bool trigger, int group, 
							  const std::vector<int>& groupList, const Logic::IPhysics *component) {

		assert(_scene);

		_isTrigger = trigger;

		// Creamos una esfera dinámica
		PxTransform pose(Vector3ToPxVec3(position));
		
		




		//PxTransform localPose(PxVec3(0, radius, 0)); // Transformación de coordenadas lógicas a coodenadas de PhysX
		PxTransform localPose( PxVec3(0, 0, 0) ); // CAMBIAR!!!!!!!!!!!!!!!!!!!!!!!!!







		// Crear esfera dinámico o cinemático
		if (kinematic)
			_actor = PxCreateKinematic(*_physxSDK, pose, geometry, material, density, localPose);
		else
			_actor = PxCreateDynamic(*_physxSDK, pose, geometry, material, density, localPose);
	
		// Transformarlo en trigger si es necesario
		if (trigger) {
			PxShape* shape;
			_actor->getShapes(&shape, 1, 0);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		// Anotar el componente lógico asociado a la entidad física
		_actor->userData = (void*) component;

		// Establecer el grupo de colisión
		PxSetGroup(*_actor, group);
		// Establecer los filtros en base al grupo de colision
		Physics::CServer::getSingletonPtr()->setupFiltering(_actor, group, groupList);

		// Añadir el actor a la escena
		_scene->addActor(*_actor);
	}

	//________________________________________________________________________

	void loadStatic(const Vector3 &position, const const physx::PxGeometry& geometry, physx::PxMaterial& material, 
					bool trigger, int group, const std::vector<int>& groupList, const Logic::IPhysics *component) {

	}

	//________________________________________________________________________

	Matrix4 CEntity::getTransform() {
		return PxTransformToMatrix4( _actor->getGlobalPose() );
	}

	//________________________________________________________________________

	void CEntity::setPosition(const Vector3& position) {
		// Transformación entre el sistema de coordenadas lógico y el de PhysX

		// En primer lugar obtenemos todas las formas del actor y calculamos el punto medio
		// de todas ellas.
		unsigned int nbShapes = _actor->getNbShapes(); // sacamos el numero de formas del actor
		PxShape** actorShapes = new PxShape* [nbShapes]; // creamos un array de shapes
		_actor->getShapes(actorShapes, nbShapes); // obtenemos todas las formas del actor
		float averageYPosition = 0; // Contendra la altura media de todos los shapes

		for(unsigned int i = 0; i < nbShapes; ++i) {
			PxGeometryType::Enum geomType = actorShapes[i]->getGeometryType();

			if(geomType == PxGeometryType::eSPHERE) {
				PxSphereGeometry sphereGeom; 
				actorShapes[i]->getSphereGeometry(sphereGeom);

				averageYPosition += sphereGeom.radius;
			}
			else if(geomType == PxGeometryType::eCAPSULE) {
				PxCapsuleGeometry capsuleGeom; 
				actorShapes[i]->getCapsuleGeometry(capsuleGeom);

				averageYPosition += capsuleGeom.halfHeight;
			}
			else if(geomType == PxGeometryType::eBOX) {
				PxBoxGeometry boxGeom; 
				actorShapes[i]->getBoxGeometry(boxGeom);

				PxVec3 halfPos(boxGeom.halfExtents);
				averageYPosition += halfPos.y;
			}
			/*else if(geomType == PxGeometryType::eTRIANGLEMESH) {
				// Deducir punto medio del mesh
			}*/
		}

		delete [] actorShapes;

		// Calculamos la altura media de todas las formas para colocar el vector
		// posicion de physx
		averageYPosition = averageYPosition / nbShapes;
		_actor->setGlobalPose( PxTransform( PxVec3(position.x, position.y + averageYPosition, position.z) ) );
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
