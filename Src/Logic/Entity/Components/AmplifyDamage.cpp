/**
@file AmplifyDamage.cpp

Contiene la implementación del componente que representa al hechizo de amplificar daño.
 
@see Logic::CAmplifyDamage

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "AmplifyDamage.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageDamageAmplifier.h"

namespace Logic {
	IMP_FACTORY(CAmplifyDamage);

	CAmplifyDamage::~CAmplifyDamage() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CAmplifyDamage::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		assert( entityInfo->hasAttribute(_spellName + "PercentDamage") );

		_newPercentDamage = entityInfo->getIntAttribute(_spellName + "PercentDamage");

		assert(_newPercentDamage != 0);
		return true;
	} // spawn
	//__________________________________________________________________

	void CAmplifyDamage::spell(){ 
		auto msg =  std::make_shared<CMessageDamageAmplifier>(_newPercentDamage);
		_entity->emitMessage(msg);
	} // spell
	//__________________________________________________________________
		
	void CAmplifyDamage::stopSpell() { 
		auto msg = std::make_shared<CMessageDamageAmplifier>(0);
		_entity->emitMessage(msg);
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

