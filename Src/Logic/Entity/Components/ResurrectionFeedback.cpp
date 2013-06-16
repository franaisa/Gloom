/**
@file ResurrectionFeedback.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownFeedback
@see Logic::ISpellFeedback

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#include "ResurrectionFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CResurrectionFeedback);

	//__________________________________________________________________

	CResurrectionFeedback::CResurrectionFeedback() : ISpellFeedback("resurection") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CResurrectionFeedback::~CResurrectionFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CResurrectionFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	} // spawn
	//__________________________________________________________________

	void CResurrectionFeedback::spell(){
		// Efecto cuando se activa la gravedad

	} // spell
	//__________________________________________________________________
		
	void CResurrectionFeedback::stopSpell(){
		// Efecto cuando se desactiva la gravedad

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic