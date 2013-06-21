/**
@file ComeBack.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CComeBackFeedback
@see Logic::ISpellFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "ComeBackFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CComeBackFeedback);

	//__________________________________________________________________

	CComeBackFeedback::CComeBackFeedback() : ISpellFeedback("comeBack"), _spellIsActive(false) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CComeBackFeedback::~CComeBackFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CComeBackFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	} // spawn
	//__________________________________________________________________

	void CComeBackFeedback::spell(){
		// Efecto cuando se activa
		if(!_spellIsActive){
			_spellIsActive = true;
		}else{
			//Si entra aqui es que me estoy teleportando atras
		}
		
	} // spell
	//__________________________________________________________________
		
	void CComeBackFeedback::stopSpell(){
		// Efecto cuando se desactiva
		_spellIsActive = false;
	} // stopSpell
	//__________________________________________________________________

}//namespace Logic