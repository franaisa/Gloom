/**
@file CoolDown.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownFeedback
@see Logic::ISpellFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "CoolDownFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CCoolDownFeedback);

	//__________________________________________________________________

	CCoolDownFeedback::CCoolDownFeedback() : ISpellFeedback("coolDown") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CCoolDownFeedback::~CCoolDownFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CCoolDownFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	} // spawn
	//__________________________________________________________________

	void CCoolDownFeedback::spell(){
		// Efecto cuando se activa la gravedad

	} // spell
	//__________________________________________________________________
		
	void CCoolDownFeedback::stopSpell(){
		// Efecto cuando se desactiva la gravedad

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic