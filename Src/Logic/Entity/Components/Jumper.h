#ifndef __Logic_Jumper_H
#define __Logic_Jumper_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la aplicación de la fuerza de salto a la entidad que ha tocado la entidad jumper.

*/
	class CJumper : public IComponent
	{
		DEC_FACTORY(CJumper);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CJumper() : IComponent(){}
		

	
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		*/
		virtual void activate();

		/**
		Este componente sólo acepta mensajes de tipo touched (por ahora).
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo TOUCHED aplicamos una fuerza de salto al jugador.
		*/
		virtual void process(CMessage *message);
	
		/**
		Aplica una fuerza a la entidad
		*/
		void applyJump(CEntity *entity);

	}; // class CJumper

	REG_FACTORY(CJumper);

} // namespace Logic

#endif // __Logic_Jumper_H
