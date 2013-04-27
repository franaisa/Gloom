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
	Este componente controla el movimiento de los ascensores.
	<p>
	El ascensor se moverá entre una posicion inicial y otra final segun
	se dispare el trigger o no.
	
    @ingroup logicGroup

	@author Jose Antonio García Yáñez
	@date Febrero, 2013
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
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	protected:

		/**
		Método llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajará cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el último tick. Son siempre constantes.
		*/
		virtual void onFixedTick(unsigned int msecs);

		/**
		Timers que controlan la elevación del ascensor.
		*/
		unsigned int _waitTime;
		unsigned int _waitTimeInFinal;

		/**
		Posició inicial y final del ascensor.
		*/
		Vector3 _positionInitial;
		Vector3 _positionFinal;

		/**
		Booleano que controla si esta activo.
		*/
		bool _active;

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

		/**
		Booleano que controla la espera para no ejecutar instrucciones inutiles.
		*/
		bool _wait;
		bool _waitInFinal;
		bool _go;

	}; // class CElevator

	REG_FACTORY(CElevator);

} // namespace Logic

#endif // __Logic_Elevator_H
