/**
@file ScreamerShieldController.h

Contiene la declaración del componente que 
controla el movimiento del escudo del screamer.

@see Logic::CScreamerShieldController
@see Logic::IComponent

@author Francisco Aisa García
@date Junio, 2013
*/

#ifndef __Logic_ScreamerShieldController_H
#define __Logic_ScreamerShieldController_H

#include "Logic/Entity/Component.h"

namespace Logic {
	class CPhysicDynamicEntity;
}

namespace Logic {
	
	/**
	
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Junio, 2013
	*/
	
	class CScreamerShieldController : public IComponent {
		DEC_FACTORY(CScreamerShieldController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamerShieldController();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CScreamerShieldController();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>SET_OWNER</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Este método debe ser usado por aquella entidad que cree a esta entidad,
		para indicar que ésta última le pertenece.

		@param owner Entidad lógica que generó a esta entidad.
		*/
		inline void setOwner(Logic::CEntity* owner);

		//__________________________________________________________________

		inline void setOffset(); 


	protected:

		virtual void onStart();

		virtual void onFixedTick(unsigned int msecs);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		float _capsuleRadius;

		float _heightShoot;

		/** Entidad dueña de esta entidad. */
		CEntity* _owner;

		CPhysicDynamicEntity* _physicComponent;

	}; // class CScreamerShieldController

	REG_FACTORY(CScreamerShieldController);

} // namespace Logic

#endif // __Logic_Life_H
