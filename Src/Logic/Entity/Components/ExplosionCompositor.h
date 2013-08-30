#ifndef __Logic_ExplosionCompositor_H
#define __Logic_ExplosionCompositor_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla el tiempo para la explosion del screamer.

*/
	class CExplosionCompositor : public IComponent
	{
		DEC_FACTORY(CExplosionCompositor);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CExplosionCompositor() : IComponent(){}
		

	
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

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
		Atributo que lleva el tiempo de explosion
		*/
		float _time;

	}; // class CExplosionCompositor

	REG_FACTORY(CExplosionCompositor);

} // namespace Logic

#endif // __Logic_ExplosionCompositor_H
