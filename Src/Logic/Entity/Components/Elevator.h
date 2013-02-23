/**
@file Elevator.h

Contiene la declaración del componente que controla el movimiento de los ascensores

@see Logic::CElevator
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/
#ifndef __Logic_Elevator_H
#define __Logic_Elevator_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el daño a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	TODO  Si la vida pasa a ser 0 que la entidad muera (poner animación de muerte?)
	y si es el jugador habrá que terminar el juego. Si la vida sigue siendo mayor 
	que 0 trás un golpe ¿poner la animación de herido?.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
*/
	class CElevator : public IComponent
	{
		DEC_FACTORY(CElevator);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CElevator() : IComponent() {}

		/**
		Método llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajará cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);
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
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);

		/**
		Nos movemos a la posición inicial
		*/
		void toInitial();
	
		/**
		Nos movemos a la posicion final
		*/
		void toFinal();

	protected:

		/**
		Posicion inicial y final del ascensor.
		*/
		Vector3 _positionInitial;
		Vector3 _positionFinal;


	}; // class CElevator

	REG_FACTORY(CElevator);

} // namespace Logic

#endif // __Logic_Elevator_H
