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

using namespace std;

namespace Logic {

	IMP_FACTORY(CHardDeathFeedback);

	//__________________________________________________________________

	CHardDeathFeedback::CHardDeathFeedback() : ISpellFeedback("hardDeath") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CHardDeathFeedback::~CHardDeathFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CHardDeathFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	} // spawn
	//__________________________________________________________________

	void CHardDeathFeedback::spell(){
		// Efecto cuando se activa la gravedad

	} // spell
	//__________________________________________________________________
		
	void CHardDeathFeedback::stopSpell(){
		// Efecto cuando se desactiva la gravedad

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic