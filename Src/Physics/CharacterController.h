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

#include <characterkinematic/PxController.h>

// REMEMBER! CONFIGURAR LOS FILTROS DEL CONTROLLER

// Predeclaración de clases para ahorrar tiempo de compilación
namespace physx {
	class PxCapsuleController;
	class PxControllerManager;
	class PxScene;
	class PxPhysics;
}

namespace Physics {
	class CCollisionManager;
}

namespace Logic {
	class CPhysicController;
}

namespace Physics {

	// Flags que controlan con que colisiona el controller
	enum CharacterControllerFlag {
		// El controller esta colisionando por los lados
		eCOLLISION_SIDES	= physx::PxControllerFlag::eCOLLISION_SIDES,
		// El controller colisiona por la parte superior
		eCOLLISION_UP		= physx::PxControllerFlag::eCOLLISION_UP,
		// El controller colisiona por la parte inferior
		eCOLLISION_DOWN		= physx::PxControllerFlag::eCOLLISION_DOWN,
	};

	/**
	
	@ingroup physicsGroup

	Para los character controllers, las capsulas no giran, tan solo podemos obtener
	la posicion.

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

		// move
		unsigned move(const Vector3 &movement, unsigned int msecs);
		
		// get position
		Vector3 getPosition();

		// set position
		void setPosition(const Vector3 &position);

		void activateSimulation();

		void deactivateSimulation();

	private:

		/** Controlador de la cápsula del controller. */
		physx::PxCapsuleController* _controller;

		// Gestor de character controllers
		physx::PxControllerManager* _controllerManager;

		// Puntero a la escena
		physx::PxScene* _scene;

		// SDK de Physx
		physx::PxPhysics* _physxSDK;

		/** Puntero al gestor de colisiones */
		CCollisionManager* _collisionManager;

		/** Filtros aplicados a este character controller */
		physx::PxControllerFilters _filters;
	}; // class CCharacterController

} // namespace Graphics

#endif // __Graphics_Entity_H
