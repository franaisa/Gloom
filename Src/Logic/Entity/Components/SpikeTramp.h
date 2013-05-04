/**
@file SpikeTramp.h

Contiene la declaración del componente que controla la aparicion de pinchos.

@see Logic::CSpikeTramp
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Mayo, 2013
*/
#ifndef __Logic_SpikeTramp_H
#define __Logic_SpikeTramp_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la aparicion de los pinchos en el mapa.
	<p>
	Cuando la trampa sea tocada, los pinchos asociados a ella saldran disparados.
	
    @ingroup logicGroup

	@author Jose Antonio García Yáñez
	@date Mayo, 2013
*/
	class CSpikeTramp : public IComponent
	{
		DEC_FACTORY(CSpikeTramp);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSpikeTramp() : IComponent() {}

		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void onActivate();

		/**
		Este componente sólo acepta mensajes de tipo touched (por ahora).
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo TOUCHED aplicamos una fuerza de salto al jugador.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	private:

		/**
		CoolDown de activación de la trampa de pinchos.
		*/
		int _coolDownTime;
		
	}; // class CSpikeTramp

	REG_FACTORY(CSpikeTramp);

} // namespace Logic

#endif // __Logic_SpikeTramp_H
