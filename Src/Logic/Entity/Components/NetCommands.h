/**
@file NetCommands.h

Contiene la declaración del componente que envia con cierta frecuencia
desde el servidor al cliente la posicion que existe en el servidor.

@author Rubén Mulero Guerrero
@date May, 2010
*/

#ifndef __Logic_NetCommands_H
#define __Logic_NetCommands_H

#include "Logic/Entity/Component.h"
#include <deque>

namespace Logic{
	class CMessageControl;
	class CMessageSyncPosition;
	class CPhysicController;
	class CInterpolation;
}


namespace Logic  {

	/**
    @ingroup logicGroup

	@author Rubén Mulero Guerrero
	@date Febrero, 2013
	*/

	class CNetCommands : public IComponent {
		DEC_FACTORY(CNetCommands);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. Inicializa el timer a 0. */
		CNetCommands() : IComponent(), _timer(0), _seqNumber(0) {}


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

		/**
		Método llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		void sendControlMessage(const std::shared_ptr<CMessageControl>& message);

		void acceptAndInterpolate(const std::shared_ptr<CMessageSyncPosition>& message);

		void calculateInterpolation(const Vector3 &myPosition, const Vector3 &srvPosition);

		void lerp();

	private:


		// =======================================================================
		//                            MIEMBROS PRIVADOS
		// =======================================================================


		/** Timer para controlar cada cuanto se manda el mensaje de sincronizacion de la posicion del cliente. */
		float _timer;

		/** Limite de tiempo para mandar el mensaje de sincronizacion. */
		float _syncPosTimeStamp;

		/** non acknowledged messages buffer */
		typedef std::pair <unsigned int,Vector3> TSyncMessage; 
		std::deque<TSyncMessage> _msgBuffer;

		/** ack sequence number */
		unsigned int _seqNumber;

		/** lerp component */
		CInterpolation* _interpolater;;

		/** shortcut for entity phisic controller */
		CPhysicController* _physicController;

		/** min and max lerp distance */
		float _minDist;
		float _maxDist;
	}; // class CUpdateClientPosition

	REG_FACTORY(CNetCommands);

} // namespace Logic

#endif
