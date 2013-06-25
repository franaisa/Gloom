/**
@file Gravity.cpp

Contiene la implementación del componente que representa al gravity.
 
@see Logic::CGravity

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "Gravity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"



namespace Logic {
	IMP_FACTORY(CGravity);

	CGravity::~CGravity() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CGravity::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		assert( entityInfo->hasAttribute(_spellName + "newGravity") );

		
		return true;
	} // spawn
	//__________________________________________________________________

	void CGravity::spell(){ 
		
	} // spell
	//__________________________________________________________________
		
	void CGravity::stopSpell() { 
		
	} // stopSpell
	//__________________________________________________________________

	

	
} // namespace Logic

