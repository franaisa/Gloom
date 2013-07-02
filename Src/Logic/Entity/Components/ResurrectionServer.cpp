/**
@file ResurrectionServer.cpp

Contiene la implementación del componente que representa al hechizo de resurrección.
 
@see Logic::CResurrectionServer

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "ResurrectionServer.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

//#include "Logic/Messages/MessageReducedCooldown.h"

namespace Logic {
	IMP_FACTORY(CResurrectionServer);

	CResurrectionServer::~CResurrectionServer() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CResurrectionServer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		//assert( entityInfo->hasAttribute(_spellName + "percentage") );


		return true;
	} // spawn
	//__________________________________________________________________

	void CResurrectionServer::spell(){ 
		//auto msg =  std::make_shared<CMessageReducedCooldown>(_percentage);
		//_entity->emitMessage(msg);
	} // spell
	//__________________________________________________________________
		
	void CResurrectionServer::stopSpell() { 
		//auto msg =  std::make_shared<CMessageReducedCooldown>(0);
		//_entity->emitMessage(msg);	
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

