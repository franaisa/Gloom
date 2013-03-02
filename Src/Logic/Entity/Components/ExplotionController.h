/**
@file ExplotionController.h

Contiene la declaración de la cabecera del componente
que controla el timer de la explosion.

@see Logic::CExplotionController
@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_ExplotionController_H
#define __Logic_ExplotionController_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	
	class CExplotionController : public IComponent {
		DEC_FACTORY(CExplotionController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto; en la clase base no hace nada. */
		CExplotionController() : IComponent(), _timer(0), _enemyHit(false) { }


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Método llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		//________________________________________________________________________

		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>explotionTime: </strong>tiempo en segundos tras el cual la granada explota.</li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTACT_ENTER</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(CMessage *message);

		//________________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Setea el puntero a la entidad que ha disparado la granada

		@param CEntity Puntero a la entidad que disparo la granada.
		*/
		void setOwner(CEntity* _owner);

	private:


		// =======================================================================
		//                            METODOS PRIVADOS
		// =======================================================================

		/** Crea una entidad GrenadeExplotion justo en el lugar en el que se encuentre la granada (_entity). */
		void createExplotion();

		// =======================================================================
		//                            CAMPOS PRIVADOS
		// =======================================================================

		/** Transcurrido este tiempo, la se destruye la entidad granada y se ejecuta la explosion. */
		float _explotionTime;

		/** Timer que controla cuando explota la granada. */
		unsigned int _timer;

		/** Entidad que ha disparado la granada. */
		CEntity* _owner;

		/** true si la granada a golpeado a un enemigo */
		bool _enemyHit;

	}; // class CExplotionController

	REG_FACTORY(CExplotionController);

} // namespace Logic

#endif
