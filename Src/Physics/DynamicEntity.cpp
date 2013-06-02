//---------------------------------------------------------------------------
// DynamicEntity.cpp
//---------------------------------------------------------------------------

/**
@file DynamicEntity.cpp

@see Physics::CDynamicEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "DynamicEntity.h"

#include <assert.h>

#include "Physics/Server.h"
#include "Physics/CollisionManager.h"
#include "Conversions.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"

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
using namespace std;

namespace Logic 
{
	class CMap;
	class IComponent;
	class CEntityFactory;
}

namespace Physics {

	CDynamicEntity::CDynamicEntity() {
		_dynamicActor = NULL;
	} // CDynamicEntity

	//________________________________________________________________________

	CDynamicEntity::~CDynamicEntity() {
		// El destructor del padre se encarga de liberar la memoria reservada
		// por el actor en PhysX
		_dynamicActor = NULL;
	} // ~CDynamicEntity

	//________________________________________________________________________

	void CDynamicEntity::load(const std::string &file, int group, const vector<int>& groupList, const Logic::IPhysics* component) {
		// Cargamos desde fichero los datos del actor
		CEntity::load(file, group, groupList, component);

		// Asignamos al actor los flags que corresponden a los rigid dinamicos
	}

	//________________________________________________________________________

	void CDynamicEntity::load(const Matrix4& transform, const PxGeometry& geometry, PxMaterial& material,
							  float density, bool kinematic, bool trigger, bool noGravity, int group, 
							  const vector<int>& groupList, const Logic::IPhysics *component) {

		assert(_scene);

		_isTrigger = trigger;

		// Creamos una esfera dinámica
		PxTransform globalPose( Matrix4ToPxTransform(transform));
		
		// Transformación de coordenadas lógicas a coodenadas de PhysX
		PxTransform shapeOffset( PxVec3(0, getLogicPivotOffset(geometry), 0) );

		// Crear esfera dinámico o cinemático
		if (kinematic)
			_actor = _dynamicActor = PxCreateKinematic(*_physxSDK, globalPose, geometry, material, density, shapeOffset);
		else
			_actor = _dynamicActor = PxCreateDynamic(*_physxSDK, globalPose, geometry, material, density, shapeOffset);
	
		// Transformarlo en trigger si es necesario
		if (trigger) {
			PxShape* shape;
			_dynamicActor->getShapes(&shape, 1, 0);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		if(noGravity)
			_dynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

		// Anotar el componente lógico asociado a la entidad física
		_dynamicActor->userData = (void*) component;

		// Establecer el grupo de colisión
		PxSetGroup(*_dynamicActor, group);
		// Establecer los filtros en base al grupo de colision
		Physics::CServer::getSingletonPtr()->setupFiltering(_dynamicActor, group, groupList);

		// Añadir el actor a la escena
		_scene->addActor(*_dynamicActor);
	}

	//________________________________________________________________________

	void CDynamicEntity::addForce(const Vector3& forceVector, ForceMode forceMode, bool autowake) {
		switch(forceMode) {
			case eFORCE:
				_dynamicActor->addForce( Vector3ToPxVec3(forceVector), PxForceMode::eFORCE, autowake );
				break;

			case eIMPULSE:
				_dynamicActor->addForce( Vector3ToPxVec3(forceVector), PxForceMode::eIMPULSE, autowake );
				break;

			case eVELOCITY_CHANGE:
				_dynamicActor->addForce( Vector3ToPxVec3(forceVector), PxForceMode::eVELOCITY_CHANGE, autowake );
				break;

			case eACCELERATION:
				_dynamicActor->addForce( Vector3ToPxVec3(forceVector), PxForceMode::eACCELERATION, autowake );
				break;
		}
	}

	//________________________________________________________________________

	void CDynamicEntity::addTorque(const Vector3& forceVector, ForceMode forceMode, bool autowake) {
		switch(forceMode) {
			case eFORCE:
				_dynamicActor->addTorque( Vector3ToPxVec3(forceVector), PxForceMode::eFORCE, autowake );
				break;

			case eIMPULSE:
				_dynamicActor->addTorque( Vector3ToPxVec3(forceVector), PxForceMode::eIMPULSE, autowake );
				break;

			case eVELOCITY_CHANGE:
				_dynamicActor->addTorque( Vector3ToPxVec3(forceVector), PxForceMode::eVELOCITY_CHANGE, autowake );
				break;

			case eACCELERATION:
				_dynamicActor->addTorque( Vector3ToPxVec3(forceVector), PxForceMode::eACCELERATION, autowake );
				break;
		}
	}

	//________________________________________________________________________

	void CDynamicEntity::disableGravity(bool state) {
		_dynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, state);
	}

	//________________________________________________________________________

	void CDynamicEntity::clearForce(ForceMode forceMode, bool autowake) {
			switch(forceMode) {
			case eFORCE:
				//_dynamicActor->clearForce(PxForceMode::eFORCE,autowake);
				//Poniendo velocidad a pelo porque el clearForce no hace lo que dice
				_dynamicActor->setLinearVelocity(PxVec3(0.0,0.0,0.0),autowake);
				break;

			case eIMPULSE:
				//_dynamicActor->clearForce(PxForceMode::eIMPULSE,autowake);
				//Poniendo velocidad a pelo porque el clearForce no hace lo que dice
				_dynamicActor->setLinearVelocity(PxVec3(0.0,0.0,0.0),autowake);
				break;
		}
	}

	//________________________________________________________________________

	bool CDynamicEntity::isKinematic() {
		return _dynamicActor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC;
	}

	//________________________________________________________________________

	void CDynamicEntity::move(const Matrix4& transform) {
		assert( isKinematic() );

		// Mover el actor tras transformar el destino a coordenadas lógicas
		_dynamicActor->setKinematicTarget( Matrix4ToPxTransform(transform) );
	}

	//________________________________________________________________________

	void CDynamicEntity::move(const Vector3& displ) {
		assert( isKinematic() );

		// Desplazar el actor
		PxTransform transform = _dynamicActor->getGlobalPose();
		transform.p += Vector3ToPxVec3(displ);
		_dynamicActor->setKinematicTarget(transform);
	}

	//________________________________________________________________________

	void CDynamicEntity::setPosition(const Vector3& position, bool makeConversionToLogicWorld) {
		if(makeConversionToLogicWorld) {
			Vector3 logicPosition = convertPhysxCoordsToLogicCoords(position);
			_actor->setGlobalPose( PxTransform( PxVec3(logicPosition.x, logicPosition.y, logicPosition.z) ) );
		}
		else {
			_actor->setGlobalPose( PxTransform( PxVec3(position.x, position.y, position.z) ) );
		}
	}
	
	//________________________________________________________________________

	void CDynamicEntity::setTransform(const Vector3 &position, const Quaternion &orientation, bool makeConversionToLogicWorld) {
		if(makeConversionToLogicWorld) {
			_actor->setGlobalPose( PxTransform(Vector3ToPxVec3( convertPhysxCoordsToLogicCoords(position)),QuaternionToPxQuat(orientation)));
		}
		else {
			_actor->setGlobalPose(  PxTransform(Vector3ToPxVec3(position),QuaternionToPxQuat(orientation)) );
		}
	}

	//________________________________________________________________________

	Vector3 CDynamicEntity::convertPhysxCoordsToLogicCoords(const Vector3& position) {
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
		return Vector3(position.x, position.y + averageYPosition, position.z);
	}


} // namespace Physics
