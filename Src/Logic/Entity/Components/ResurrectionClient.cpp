/**
@file ResurrectionClient.cpp

Contiene la implementación del componente que representa al hechizo de resurrección.
 
@see Logic::CResurrectionClient

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "ResurrectionClient.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

//#include "Logic/Messages/MessageReducedCooldown.h"

namespace Logic {
	IMP_FACTORY(CResurrectionClient);

	CResurrectionClient::~CResurrectionClient() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CResurrectionClient::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		//assert( entityInfo->hasAttribute(_spellName + "percentage") );


		return true;
	} // spawn
	//__________________________________________________________________

	void CResurrectionClient::spell(){ 
		//auto msg =  std::make_shared<CMessageReducedCooldown>(_percentage);
		//_entity->emitMessage(msg);
	} // spell
	//__________________________________________________________________
		
	void CResurrectionClient::stopSpell() { 
		//auto msg =  std::make_shared<CMessageReducedCooldown>(0);
		//_entity->emitMessage(msg);	
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

