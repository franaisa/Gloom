/**
@file HardDeathAmmo.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownAmmo
@see Logic::ISpellAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "HardDeathAmmo.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "HardDeathAmmo.h"
#include "HardDeathFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CHardDeathAmmo);

	//__________________________________________________________________

	CHardDeathAmmo::CHardDeathAmmo() : ISpellAmmo("hardDeath"){
		// Nada que hacer
	}

	//__________________________________________________________________

	CHardDeathAmmo::~CHardDeathAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CHardDeathAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellAmmo::spawn(entity, map, entityInfo) ) return false;

		_friend = _entity->getComponent<Logic::CHardDeathAmmo>("CHardDeathAmmo");
		/*if(!_friend)
			_friend = _entity->getComponent<Logic::CHardDeathFeedback>("CHardDeathFeedback");*/

		return true;
	}
	//__________________________________________________________________



}//namespace Logic