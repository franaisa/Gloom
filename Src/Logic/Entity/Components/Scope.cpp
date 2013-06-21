/**
@file Scope.cpp

Contiene la implementación del componente que representa al hechizo scope.
 
@see Logic::CScope

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "Scope.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

//#include "Logic/Messages/MessageChangeGravity.h"

namespace Logic {
	IMP_FACTORY(CScope);

	CScope::~CScope() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CScope::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		/*assert( entityInfo->hasAttribute(_spellName + "newGravity") );

		_newGravity = entityInfo->getFloatAttribute(_spellName + "newGravity");

		assert( entityInfo->hasAttribute("gravity") && "Error: No se ha definido el atributo gravity en el mapa" );
		_defaultGravity = entityInfo->getFloatAttribute("gravity");

		assert(_newGravity == 0);*/
		return true;
	} // spawn
	//__________________________________________________________________

	void CScope::spell(){ 
		/*auto msg =  std::make_shared<CMessageChangeGravity>(_newGravity);
		_entity->emitMessage(msg);*/
	} // spell
	//__________________________________________________________________
		
	void CScope::stopSpell() { 
		/*auto msg = std::make_shared<CMessageChangeGravity>(_defaultGravity);
		_entity->emitMessage(msg);*/
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

