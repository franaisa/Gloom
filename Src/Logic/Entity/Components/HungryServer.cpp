/**
@file HungryServer.cpp

Contiene la implementación del componente que representa al hungry.
 
@see Logic::CHungryServer

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "HungryServer.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageSpellHungry.h"

namespace Logic {
	IMP_FACTORY(CHungryServer);

	CHungryServer::~CHungryServer() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CHungryServer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		//Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_spellName + "Percentage") );

		_percentage = entityInfo->getFloatAttribute(_spellName + "Percentage");


		return true;
	} // spawn
	//__________________________________________________________________

	void CHungryServer::spell(){ 
		
		auto msg =  std::make_shared<CMessageSpellHungry>(_percentage);
		_entity->emitMessage(msg);

	} // spell
	//__________________________________________________________________
		
	void CHungryServer::stopSpell() { 
	
		auto msg =  std::make_shared<CMessageSpellHungry>(0);
		_entity->emitMessage(msg);

	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

