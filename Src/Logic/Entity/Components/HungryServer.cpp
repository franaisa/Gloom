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

namespace Logic {
	IMP_FACTORY(CHungryServer);

	CHungryServer::~CHungryServer() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CHungryServer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		//assert( entityInfo->hasAttribute(_spellName + "PercentageCooldown") );

		//_percentage = entityInfo->getFloatAttribute(_spellName + "PercentageCooldown");


		return true;
	} // spawn
	//__________________________________________________________________

	void CHungryServer::spell(){ 
		
		

	} // spell
	//__________________________________________________________________
		
	void CHungryServer::stopSpell() { 
	
		
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

