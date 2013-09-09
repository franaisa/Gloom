#ifndef __Logic_SpellsManagerServer_H
#define __Logic_SpellsManagerServer_H

#include "Logic/Entity/Component.h"

#include <vector>

//declaración de la clase

namespace Logic {
	class ISpell;
}

namespace Logic
{
/**
	Este componente controla la activacion/desactivacion de los spells que tenemos.
*/
	class CSpellsManagerServer : public IComponent
	{
		DEC_FACTORY(CSpellsManagerServer);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSpellsManagerServer();
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool accept(const std::shared_ptr<CMessage>& message);
		virtual void process(const std::shared_ptr<CMessage>& message);
		
	private:

		std::vector<ISpell*> _spells;

	}; // class CSpellsManagerServer

	REG_FACTORY(CSpellsManagerServer);

} // namespace Logic

#endif // __Logic_Shoot_H
