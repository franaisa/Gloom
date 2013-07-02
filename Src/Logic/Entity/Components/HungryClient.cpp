/**
@file HungryClient.cpp

Contiene la implementación del componente que representa al hungry.
 
@see Logic::CHungryClient

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "HungryClient.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

namespace Logic {
	IMP_FACTORY(CHungryClient);

	CHungryClient::~CHungryClient() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CHungryClient::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		return true;
	} // spawn
	//__________________________________________________________________

	void CHungryClient::spell(){ 
		// feedback de cooldown reducio

	} // spell
	//__________________________________________________________________
		
	void CHungryClient::stopSpell() { 
		// fin feedback de cooldown reducio

	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

