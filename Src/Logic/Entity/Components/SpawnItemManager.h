/**
@file SpawnItemManager.h

Contiene la declaración de la cabecera del componente
que controla el comportamiento de los items.

@see Logic::CSpawnItemManager
@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_SpawnItemManager_H
#define __Logic_SpawnItemManager_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	Este componente es el encargado de gestionar el comportamiento
	de los items; tanto los tiempos de respawn como los efectos
	que provoca cada uno.

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	
	class CSpawnItemManager : public IComponent {
		DEC_FACTORY(CSpawnItemManager);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto; en la clase base no hace nada. */
		CSpawnItemManager() : IComponent(), _isRespawning(false), _timer(0), _weaponType(-1) {}


		// =======================================================================
		//                   METODOS HEREDADOS DE ICOMPONENT
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
			<li><strong>id:</strong> Tipo del item: orbe, arma, armadura... </li>
			<li><strong>weaponType:</strong> Si el id es weapon o weaponAmmo esta etiqueta identifica de que arma se trata. </li>
			<li><strong>reward:</strong> Puntos de beneficio que proporciona el objeto. </li>
			<li><strong>respawnTime:</strong> Tiempo de respawn en segundos. </li>
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
			<li>TOUCHED</li>
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
		//                           METODOS PROPIOS
		// =======================================================================


		/**
		Cuando se coge un objeto se llama a esta funcion, que es la encargada de 
		mandar los mensajes que correspondan a la entidad que ha cogido el objeto.

		@param actor Entidad que ha cogido el item.
		*/
		void itemGrabbed(CEntity* actor);

	private:


		// =======================================================================
		//                            CAMPOS PRIVADOS
		// =======================================================================


		/** Id del item que controla este spawn manager */
		std::string _id;

		/** 
		Tipo del arma o la municion de este item. Si no se trata de municion o de un arma
		su valor es -1.
		*/
		int _weaponType;

		/** 
		Cantidad de puntos que ofrece este item al ser cogido (los puntos pueden indicar
		balas, vida, armadura...
		*/
		int _reward;

		/** Tiempo de respawn de este item en segundos. */
		float _respawnTime;

		/** Reloj que controla el tiempo que le queda a este item para respawnear. */
		unsigned int _timer;

		/** true si este item esta en fase de respawn. */
		bool _isRespawning;

	}; // class CSpawnItemManager

	REG_FACTORY(CSpawnItemManager);

} // namespace Logic

#endif
