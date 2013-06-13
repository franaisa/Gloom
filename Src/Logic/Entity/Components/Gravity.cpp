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

#include "Logic/Messages/MessageChangeGravity.h"

namespace Logic {
	IMP_FACTORY(CGravity);

	CGravity::~CGravity() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CGravity::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		assert( entityInfo->hasAttribute(_spellName + "newGravity") );

		_newGravity = entityInfo->getFloatAttribute(_spellName + "newGravity");

		assert( entityInfo->hasAttribute("gravity") && "Error: No se ha definido el atributo gravity en el mapa" );
		_defaultGravity = entityInfo->getFloatAttribute("gravity");

		assert(_newGravity == 0);
		return true;
	} // spawn
	//__________________________________________________________________

	void CGravity::spell(){ 
		auto msg =  std::make_shared<CMessageChangeGravity>(_newGravity);
		_entity->emitMessage(msg);
	} // spell
	//__________________________________________________________________
		
	void CGravity::stopSpell() { 
		auto msg = std::make_shared<CMessageChangeGravity>(_defaultGravity);
		_entity->emitMessage(msg);	
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

