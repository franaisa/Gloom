/**
@file Hacksaw.h

Contiene la declaración del componente que controla el movimiento de la sierra mecánica

@see Logic::CHacksaw
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/
#ifndef __Logic_Hacksaw_H
#define __Logic_Hacksaw_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la aparicion de los pinchos en el mapa.
	<p>
	Los pinchos aparecerán cada cierto tiempo.
	
    @ingroup logicGroup

	@author Jose Antonio García Yáñez
	@date Febrero, 2013
*/
	class CHacksaw : public IComponent
	{
		DEC_FACTORY(CHacksaw);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CHacksaw() : IComponent() {}

		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void activate();

		/**
		Este componente sólo acepta mensajes de tipo touched (por ahora).
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo TOUCHED aplicamos una fuerza de salto al jugador.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);
	

	protected:

		/**
		Método llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajará cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);
		
		/**
		Posició inicial y final del ascensor.
		*/
		Vector3 _positionInitial;
		Vector3 _positionFinal;

		/**
		Controla si vamos hacia la posicion final o no
		*/
		bool _toFinal;
	
		/**
		Dirección inicial y final del ascensor.
		*/
		Vector3 _directionFinal;
		Vector3 _directionInitial;

		/**
		Velocidad del ascensor
		*/
		float _velocity;

	}; // class CHacksaw

	REG_FACTORY(CHacksaw);

} // namespace Logic

#endif // __Logic_Hacksaw_H
