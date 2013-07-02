/**
@file ShieldClient.cpp

Contiene la implementación del componente que representa al hechizo shield.
 
@see Logic::CShieldClient

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "ShieldClient.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

namespace Logic {
	IMP_FACTORY(CShieldClient);

	CShieldClient::~CShieldClient() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CShieldClient::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;
	
		return true;
	} // spawn
	//__________________________________________________________________

	void CShieldClient::spell(){ 

		

		

	} // spell
	//__________________________________________________________________
		
	void CShieldClient::stopSpell() {
		
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

