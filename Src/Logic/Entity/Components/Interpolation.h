/**
@file Interpolation.h

Contiene la declaración del componente que realiza la interpolacion
del movimiento cuando el servidor nos informa de la posicion que deberiamos
tener.

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_Interpolation_H
#define __Logic_Interpolation_H

#include "Logic/Entity/Component.h"

namespace Logic  {

	/**
    @ingroup logicGroup

	Clase encargada de recolocar al jugador en caso de no estar sincronizados
	con el servidor.

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CInterpolation : public IComponent {
		DEC_FACTORY(CInterpolation);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto */
		CInterpolation() : IComponent() {}


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa. Actualmente este componente no lee nada para inicializarse.

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
			<li>SYNC_POSITION</li>
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

	}; // class CInterpolation

	REG_FACTORY(CInterpolation);

} // namespace Logic

#endif // __Logic_Life_H
