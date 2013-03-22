//---------------------------------------------------------------------------
// CharacterController.h
//---------------------------------------------------------------------------

/**
@file CharacterController.h

Contiene la declaración de la clase que encapsula al controlador de PhysX.

@see Graphics::CCharacterController
@see physx::PxController

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_CharacterController_H
#define __Physics_CharacterController_H

#include "BaseSubsystems/Math.h"
#include <vector>

#include <characterkinematic/PxController.h>

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
		eCOLLISION_SIDES = physx::PxControllerFlag::eCOLLISION_SIDES,
		// El controller colisiona por la parte superior
		eCOLLISION_UP = physx::PxControllerFlag::eCOLLISION_UP,
		// El controller colisiona por la parte inferior
		eCOLLISION_DOWN	= physx::PxControllerFlag::eCOLLISION_DOWN,
	};

	/**
	El objetivo de esta clase es abstraer el comportamiento de los character
	controllers. Por debajo se apoya en el uso del controlador de PhysX 
	(PxController).

	Notar que la cápsula física del jugador no gira en PhysX, tan solo se mueve.
	Por este motivo no podemos ni obtener ni fijar la orientación de la cápsula,
	tan solo la posición.

	Es posible aplicar una serie de filtros al controlador que luego influyen
	en el movimiento kinemático y demás. De momento no se usa.
	
	@ingroup physicsGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CCharacterController {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		/** Constructor por defecto. */
		CCharacterController();

		//__________________________________________________________________

		/** Destructor. */
		~CCharacterController();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Método que se encarga de inicializar por completo el controlador. El
		constructor realmente no hace nada.

		@param position Posición donde queremos situar la cápsula.
		@param radius Radio de la cápsula.
		@param height Altura de la cápsula.
		@param group Grupo de colisión al que pertenece la cápsula.
		@param groupList Grupos de colisión con los que queremos que la
		cápsula interactue.
		@param component Puntero al componente lógico asociado al controlador
		de la cápsula.
		*/
		void load(const Vector3 &position, float radius, float height, 
				  int group, const std::vector<int>& groupList, 
	              const Logic::CPhysicController* component);

		//__________________________________________________________________

		/**
		Mueve a la cápsula en la dirección que especifiquemos.

		@param movement Vector que indica la dirección en la que nos queremos
		mover.
		@param msecs Cantidad de milisegundos que queremos que se produzca el
		desplazamiento.
		@return Flags de colisión del controller.
		*/
		unsigned move(const Vector3 &movement, unsigned int msecs);

		//__________________________________________________________________
		
		/**
		Devuelve la posición del controlador.

		@return La posición de la cápsula.
		*/
		Vector3 getPosition();

		//__________________________________________________________________

		/**
		Setea la posición del controlador. Ojo con este método, ya que transporta
		a la cápsula de un sitio a otro.

		@param position Posicion en la que queremos situar la cápsula.
		*/
		void setPosition(const Vector3 &position);

		//__________________________________________________________________

		/** Activa la simulación física de la cápsula. */
		void activateSimulation();

		//__________________________________________________________________

		/** Desactiva la simulación física de la cápsula. */
		void deactivateSimulation();

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Controlador de la cápsula del controller. */
		physx::PxCapsuleController* _controller;

		/** Gestor de controladores en PhysX. */
		physx::PxControllerManager* _controllerManager;

		/** Puntero a la escena física. */
		physx::PxScene* _scene;

		/** Puntero al core de PhysX. */
		physx::PxPhysics* _physxSDK;

		/** Puntero al gestor de colisiones */
		CCollisionManager* _collisionManager;

		/** Filtros aplicados a este character controller */
		physx::PxControllerFilters _filters;
	}; // class CCharacterController

} // namespace Physics

#endif // __Physics_CharacterController_H
