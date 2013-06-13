/**
@file CoolDown.cpp

Contiene la implementación del componente que representa al coolDown.
 
@see Logic::CCoolDown

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "CoolDown.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageReducedCooldown.h"

namespace Logic {
	IMP_FACTORY(CCoolDown);

	CCoolDown::~CCoolDown() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CCoolDown::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_spellName + "percentage") );

		_percentage = entityInfo->getFloatAttribute(_spellName + "percentage");

		return true;
	} // spawn
	//__________________________________________________________________

	void CCoolDown::spell(){ 
		auto msg =  std::make_shared<CMessageReducedCooldown>(_percentage);
		_entity->emitMessage(msg);
	} // spell
	//__________________________________________________________________
		
	void CCoolDown::stopSpell() { 
		auto msg =  std::make_shared<CMessageReducedCooldown>(0);
		_entity->emitMessage(msg);	
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

