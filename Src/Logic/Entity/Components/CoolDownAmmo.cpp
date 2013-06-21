/**
@file CoolDown.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownAmmo
@see Logic::ISpellAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/



#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "CoolDownAmmo.h"
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

		_friend[_friends] = _entity->getComponent<Logic::CCoolDown>("CCoolDown");
		if(_friend[_friends]) ++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CCoolDownFeedback>("CCoolDownFeedback");
		if(_friend[_friends]) ++_friends;
		if(_friends == 0) assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}
	//__________________________________________________________________

	void CCoolDownAmmo::spell() {
		ISpellAmmo::spell();

		_spellIsActive = true;

	} // spell
	//__________________________________________________________________

	void CCoolDownAmmo::stopSpell() {
		ISpellAmmo::stopSpell();

		_spellIsActive = false;
	} // stopSpell
	//__________________________________________________________________

}//namespace Logic