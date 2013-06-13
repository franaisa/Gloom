/**
@file CoolDown.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownAmmo
@see Logic::ISpellAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "CoolDownAmmo.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "CoolDown.h"
#include "CoolDownFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CCoolDownAmmo);

	//__________________________________________________________________

	CCoolDownAmmo::CCoolDownAmmo() : ISpellAmmo("coolDown"){
		// Nada que hacer
	}

	//__________________________________________________________________

	CCoolDownAmmo::~CCoolDownAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CCoolDownAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellAmmo::spawn(entity, map, entityInfo) ) return false;

		_friend = _entity->getComponent<Logic::CCoolDown>("CCoolDown");
		if(!_friend)
			_friend = _entity->getComponent<Logic::CCoolDownFeedback>("CCoolDownFeedback");

		return true;
	}
	//__________________________________________________________________



}//namespace Logic