/**
@file Light.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CLight
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Marzo, 2013
*/
#ifndef __Logic_Light_H
#define __Logic_Light_H

#include "Graphics/Light.h"
#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la luz que tiene un objeto. 
	
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Marzo, 2013
*/
	class CLight : public IComponent
	{
		DEC_FACTORY(CLight);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLight() : IComponent() {}

		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		mensajes aceptados por el componente
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		

	protected:

		/**
		Método llamado en cada frame que actualiza el estado del componente de luz.
				

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		//virtual void onTick(unsigned int msecs);

		/**
		Puntero al objeto luz
		*/
		Graphics::CLight *_light;

	}; // class CLight

	REG_FACTORY(CLight);

} // namespace Logic

#endif // __Logic_Light_H
