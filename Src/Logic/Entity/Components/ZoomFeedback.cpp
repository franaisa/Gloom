/**
@file CoolDown.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownFeedback
@see Logic::ISpellFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "ZoomFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CZoomFeedback);

	//__________________________________________________________________

	CZoomFeedback::CZoomFeedback() : ISpellFeedback("zoom") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CZoomFeedback::~CZoomFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CZoomFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	} // spawn
	//__________________________________________________________________

	void CZoomFeedback::spell(){
		// Efecto cuando se activa la gravedad

	} // spell
	//__________________________________________________________________
		
	void CZoomFeedback::stopSpell(){
		// Efecto cuando se desactiva la gravedad

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic