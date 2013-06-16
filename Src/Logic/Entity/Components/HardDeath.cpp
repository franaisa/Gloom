/**
@file CoolDown.cpp

Contiene la implementación del componente que representa al coolDown.
 
@see Logic::CCoolDown

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "HardDeath.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageReducedCooldown.h"

namespace Logic {
	IMP_FACTORY(CHardDeath);

	CHardDeath::~CHardDeath() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CHardDeath::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		//assert( entityInfo->hasAttribute(_spellName + "percentage") );


		return true;
	} // spawn
	//__________________________________________________________________

	void CHardDeath::spell(){ 
		//auto msg =  std::make_shared<CMessageReducedCooldown>(_percentage);
		//_entity->emitMessage(msg);
	} // spell
	//__________________________________________________________________
		
	void CHardDeath::stopSpell() { 
		//auto msg =  std::make_shared<CMessageReducedCooldown>(0);
		//_entity->emitMessage(msg);	
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

