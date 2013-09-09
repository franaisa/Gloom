#ifndef __Logic_SpellsManagerClient_H
#define __Logic_SpellsManagerClient_H

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
	class CSpellsManagerClient : public IComponent
	{
		DEC_FACTORY(CSpellsManagerClient);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSpellsManagerClient();
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool accept(const std::shared_ptr<CMessage>& message);
		virtual void process(const std::shared_ptr<CMessage>& message);
		
	private:

		std::vector<ISpell*> _spells;

	}; // class CSpellsManagerClient

	REG_FACTORY(CSpellsManagerClient);

} // namespace Logic

#endif // __Logic_Shoot_H
