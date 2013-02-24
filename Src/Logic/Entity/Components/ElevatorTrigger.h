/**
@file ElevatorTrigger.h

Contiene la declaración del componente que controla el movimiento de los ascensores

@see Logic::CElevatorTrigger
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/
#ifndef __Logic_ElevatorTrigger_H
#define __Logic_ElevatorTrigger_H

#include "Logic/Entity/Component.h"
#include <ostream>

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
	class CElevatorTrigger : public IComponent
	{
		DEC_FACTORY(CElevatorTrigger);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CElevatorTrigger() : IComponent() {}

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

	protected:

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
		Nombre y puntero para lincar a la entidad dinamica sin trigger
		*/
		std::string _entityLink;
		CEntity* _elevatorLink;

		/**
		Velocidad del ascensor
		*/
		float _velocity;

	}; // class CElevator

	REG_FACTORY(CElevatorTrigger);

} // namespace Logic

#endif // __Logic_ElevatorTrigger_H
