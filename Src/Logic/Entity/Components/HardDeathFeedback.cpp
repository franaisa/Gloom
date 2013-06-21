/**
@file HardDeathFeedback.cpp

Contiene la implementacion del componente
de hechizo de hardDeath.

@see Logic::CHardDeathFeedback
@see Logic::ISpellFeedback

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#include "HardDeathFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageCreateParticle.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CHardDeathFeedback);

	//__________________________________________________________________

	CHardDeathFeedback::CHardDeathFeedback() : ISpellFeedback("hardDeath"),_explotionRadius(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CHardDeathFeedback::~CHardDeathFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CHardDeathFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute(_spellName + "ExplotionRadius") );

		_explotionRadius = entityInfo->getFloatAttribute(_spellName + "ExplotionRadius");
		return true;
	} // spawn
	//__________________________________________________________________

	void CHardDeathFeedback::spell(){
		
		shared_ptr<CMessageCreateParticle> particleMsg = make_shared<CMessageCreateParticle>();
		particleMsg->setPosition(_entity->getPosition());
		particleMsg->setParticle("ExplosionParticle");
		_entity->emitMessage(particleMsg);
	} // spell
	//__________________________________________________________________
		
	void CHardDeathFeedback::stopSpell(){
		

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic