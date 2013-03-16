//---------------------------------------------------------------------------
// CharacterController.h
//---------------------------------------------------------------------------

/**
@file CharacterController.h

@see Graphics::CCharacterController

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_CharacterController_H
#define __Physics_CharacterController_H

#include "BaseSubsystems/Math.h"
#include <vector>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace physx {
	class PxCapsuleController;
	class PxControllerManager;
	class PxScene;
	class PxPhysics;
};

namespace Physics {
	class CCollisionManager;
	class CErrorManager;
};

namespace Logic {
	class CPhysicController;
};

namespace Physics {
	/**
	
	@ingroup physicsGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	class CCharacterController {
	public:

		CCharacterController::CCharacterController();

		virtual ~CCharacterController();

		void load(const Vector3 &position, float radius, float height, 
				  int group, const std::vector<int>& groupList, 
	              const Logic::CPhysicController* component);

	protected:

		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float msecs);

		/** Controlador de la cápsula del controller. */
		physx::PxCapsuleController* _controller;

		// Gestor de character controllers
		physx::PxControllerManager* _controllerManager;

		// Puntero a la escena
		physx::PxScene* _scene;

		// SDK de Physx
		physx::PxPhysics* _physxSDK;

		CCollisionManager* _collisionManager;
	}; // class CCharacterController

} // namespace Graphics

#endif // __Graphics_Entity_H
