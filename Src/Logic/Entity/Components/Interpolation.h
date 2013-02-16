/**
@file Interpolation.h

Contiene la declaración del componente que realiza la interpolacion
del movimiento cuando el servidor nos informa de la posicion que deberiamos
tener

@author Francisco Aisa García
@date Octubre, 2010
*/
#ifndef __Logic_Interpolation_H
#define __Logic_Interpolation_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic  {

	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CInterpolation : public IComponent {
		DEC_FACTORY(CInterpolation);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CInterpolation() : IComponent() {}

		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);

	}; // class CInterpolation

	REG_FACTORY(CInterpolation);

} // namespace Logic

#endif // __Logic_Life_H
