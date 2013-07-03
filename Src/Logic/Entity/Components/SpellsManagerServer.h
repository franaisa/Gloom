#ifndef __Logic_SpellsManagerServer_H
#define __Logic_SpellsManagerServer_H

#include "Logic/Entity/Component.h"
#include "SpellType.h"

#include <vector>

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la activacion/desactivacion de las armas que tenemos.
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

		virtual void onStart();
		
	private:

		void addComponent();

		SpellType::Enum _primarySpell;

		SpellType::Enum _secondarySpell;


		CEntity *entitySpawn;
		CMap *mapSpawn;
		const Map::CEntity *entityInfoSpawn;
	}; // class CShoot

	REG_FACTORY(CSpellsManagerServer);

} // namespace Logic

#endif // __Logic_Shoot_H
