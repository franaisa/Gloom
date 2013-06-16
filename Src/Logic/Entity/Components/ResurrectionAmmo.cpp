/**
@file ResurrectionAmmo.cpp

Contiene la implementacion del componente
de hechizo de resurrección.

@see Logic::CResurrectionAmmo
@see Logic::ISpellAmmo

@author Jaime Chapinal Cervnates
@date Junio, 2013
*/

#include "ResurrectionAmmo.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Resurrection.h"
#include "ResurrectionFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CResurrectionAmmo);

	//__________________________________________________________________

	CResurrectionAmmo::CResurrectionAmmo() : ISpellAmmo("resurection"){
		// Nada que hacer
	}

	//__________________________________________________________________

	CResurrectionAmmo::~CResurrectionAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CResurrectionAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellAmmo::spawn(entity, map, entityInfo) ) return false;

		/*_friend = _entity->getComponent<Logic::CCoolDown>("CCoolDown");
		if(!_friend)
			_friend = _entity->getComponent<Logic::CCoolDownFeedback>("CCoolDownFeedback");
			*/
		return true;
	}
	//__________________________________________________________________



}//namespace Logic