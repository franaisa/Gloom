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

	/**
    @ingroup logicGroup

	@author Francisco Aisa García
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
			<li><strong>syncPosTimeStamp:</strong> Tiempo de recolocacion del player </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	private:


		// =======================================================================
		//                            MIEMBROS PRIVADOS
		// =======================================================================


		/** Timer para controlar cada cuanto se manda el mensaje de sincronizacion de la posicion del cliente. */
		float _timer;

		/** Limite de tiempo para mandar el mensaje de sincronizacion. */
		float _syncPosTimeStamp;
	}; // class CUpdateClientPosition

	REG_FACTORY(CUpdateClientPosition);

} // namespace Logic

#endif
