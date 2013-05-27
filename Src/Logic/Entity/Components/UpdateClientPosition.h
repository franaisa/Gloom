/**
@file UpdateClientPosition.h

Contiene la declaración del componente que envia con cierta frecuencia
desde el servidor al cliente la posicion que existe en el servidor.

@author Francisco Aisa García
@date Octubre, 2010
*/

#ifndef __Logic_UpdateClientPosition_H
#define __Logic_UpdateClientPosition_H

#include "Logic/Entity/Component.h"

namespace Logic  {

	//forward declarations
	class CMessageSyncPosition;
	class CPhysicController;

	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@author Rubén Mulero Guerrero
	@date Febrero, 2013
	*/

	class CUpdateClientPosition : public IComponent {
		DEC_FACTORY(CUpdateClientPosition);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. Inicializa el timer a 0. */
		CUpdateClientPosition() : IComponent(), _timer(0) {}


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>syncPosTimeStamp:</strong> Tiempo de recolocacion del player </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>SYNC_POSITION</li>
			<li>CONTROL</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//________________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	protected:

		void sendACKMessage(unsigned int sequenceNumber);

		void updateClientPosition(const std::shared_ptr<CMessageSyncPosition> &message);

	private:


		// =======================================================================
		//                            MIEMBROS PRIVADOS
		// =======================================================================


		/** Timer para controlar cada cuanto se manda el mensaje de sincronizacion de la posicion del cliente. */
		float _timer;

		/** Limite de tiempo para mandar el mensaje de sincronizacion. */
		float _syncPosTimeStamp;


		CPhysicController *_physicController;
	}; // class CUpdateClientPosition

	REG_FACTORY(CUpdateClientPosition);

} // namespace Logic

#endif
