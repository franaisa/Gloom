/**
@file ClientRespawn.h

Contiene la declaración del componente que se encarga de respawnearnos
en el cliente.

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_ClientRespawn_H
#define __Logic_ClientRespawn_H

#include "Logic/Entity/Component.h"

namespace Logic  {

	/**
    @ingroup logicGroup

	Componente encargado de recibir los mensajes de muerte y respawn del servidor
	para activar y desactivar los componentes del cliente segun convenga.

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CClientRespawn : public IComponent {
		DEC_FACTORY(CClientRespawn);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto */
		CClientRespawn() : IComponent() {}


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>PLAYER_DEAD</li>
			<li>PLAYER_SPAWN</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		virtual void activate(){}

		//________________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	}; // class CClientRespawn

	REG_FACTORY(CClientRespawn);

} // namespace Logic

#endif // __Logic_Life_H
