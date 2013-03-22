//---------------------------------------------------------------------------
// DynamicEntity.h
//---------------------------------------------------------------------------

/**
@file DynamicEntity.h

@see Physics::CDynamicEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_DynamicEntity_H
#define __Physics_DynamicEntity_H

#include "BaseSubsystems/Math.h"
#include "Physics/Entity.h"

#include <geometry/PxGeometry.h>
#include <PxMaterial.h>
#include <PxForceMode.h>

#include <vector>
#include <string>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace physx {
	class PxScene;
	class PxPhysics;
	class PxRigidDynamic;
}

namespace Physics {
	class CCollisionManager;
	class CErrorManager;
}

namespace Logic {
	class CPhysicEntity;
	class IPhysics;
}

namespace Physics {

	enum ForceMode {
		eFORCE				= physx::PxForceMode::eFORCE,
		eIMPULSE			= physx::PxForceMode::eIMPULSE,
		eVELOCITY_CHANGE	= physx::PxForceMode::eVELOCITY_CHANGE,
		eACCELERATION		= physx::PxForceMode::eACCELERATION
	};

	/**

	IMPORTANTE: Las entidades físicas tienen orientación (a diferencia de
	los controllers). Actualmente solo soportamos métodos de seteo de posición.
	Deberiamos permitir setear la orientación también.
	
	LAS ENTIDADES FISICAS TIENEN ORIENTACION, NO SON COMO LOS CONTROLLERS!
	PERMITIR FIJAR ESTOS PARAMETROS

	De momento solo un shape por entidad, pero en el futuro un vector de shapes
	cada una con sus propiedades

	@ingroup physicsGroup

	Para los character controllers, las capsulas no giran, tan solo podemos obtener
	la posicion.

	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	class CDynamicEntity : public CEntity {
	public:

		CDynamicEntity::CDynamicEntity();

		virtual ~CDynamicEntity();

		void load(const Vector3 &position, const physx::PxGeometry& geometry, physx::PxMaterial& material, 
				  float density, bool kinematic, bool trigger, int group, 
				  const std::vector<int>& groupList, const Logic::IPhysics *component);

		// Cargar desde fichero
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics *component);
						 
		void addForce(const Vector3& forceVector, ForceMode forceMode = ForceMode::eFORCE, bool autowake = true);

		void addTorque(const Vector3& forceVector, ForceMode forceMode = ForceMode::eFORCE, bool autowake = true);

		void disableGravity(bool state);

		bool isKinematic();

		// Movimiento kinematico. El actor debe ser dinamico y kinematico (en caso contrario peta).
		void move(const Matrix4 &transform);

		void move(const Vector3& displ);

		//activate/deactivate gravity

		// Deberiamos tener un SET TRANSFORM Y UN GET POSITION!!!!!!!!!!!!!!!!

		// set position
		void setPosition(const Vector3 &position, bool makeConversionToLogicWorld);

	private:

		/** Controlador de la cápsula del controller. */
		physx::PxRigidDynamic* _dynamicActor;

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
