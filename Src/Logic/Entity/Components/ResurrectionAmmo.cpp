/**
@file ResurrectionAmmo.cpp

Contiene la implementacion del componente
de hechizo de resurrección.

@see Logic::CResurrectionAmmo
@see Logic::ISpellAmmo

@author Jaime Chapinal Cervnates
@date Junio, 2013
*/

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "ResurrectionAmmo.h"
#include "Resurrection.h"
#include "ResurrectionFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CResurrectionAmmo);

	//__________________________________________________________________

	CResurrectionAmmo::CResurrectionAmmo() : ISpellAmmo("resurrection"){
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
		_friend[_friends] = _entity->getComponent<Logic::CResurrection>("CResurrection");
		if(_friend[_friends]) ++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CResurrectionFeedback>("CResurrectionFeedback");
		if(_friend[_friends]) ++_friends;
		if(_friends == 0) assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}
	//__________________________________________________________________



}//namespace Logic