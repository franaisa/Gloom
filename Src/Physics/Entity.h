//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

@see Physics::CEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_Entity_H
#define __Physics_Entity_H

#include "BaseSubsystems/Math.h"

#include <geometry/PxGeometry.h>
#include <PxMaterial.h>
#include <PxForceMode.h>

#include <vector>
#include <string>

// REMEMBER! CONFIGURAR LOS FILTROS DEL CONTROLLER

// Predeclaración de clases para ahorrar tiempo de compilación
namespace physx {
	class PxScene;
	class PxPhysics;
	class PxRigidActor;
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

	/**
	
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
	class CEntity {
	public:

		CEntity::CEntity();

		virtual ~CEntity();

		/**
		Devuelve la posición y rotación de una entidad física.

		@param actor Actor físico del que se desea conocer la posición y orientación.
		@return Matriz 4x4 con la posición y orientación de la entidad.
		 */
		Matrix4 getTransform() const;

		// Deberiamos tener un SET TRANSFORM Y UN GET POSITION!!!!!!!!!!!!!!!!

		void activateSimulation();

		void deactivateSimulation();

		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics *component) = 0;

	protected:

		physx::PxRigidActor* deserializeRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics *component);

		float getLogicPivotOffset(const physx::PxGeometry& geometry);

		/** Rigidbody. IMPORTANTE: TIENE QUE SER INICIALIZADO POR LA CLASE HIJA. */
		physx::PxRigidActor* _actor;

		// Puntero a la escena
		physx::PxScene* _scene;

		// SDK de Physx
		physx::PxPhysics* _physxSDK;

		/** Puntero al gestor de colisiones */
		CCollisionManager* _collisionManager;

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
