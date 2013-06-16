/**
@file AmplifyDamage.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CAmplifyDamage
@see Logic::ISpellFeedback

@author Jaime Chapinal Cervnates
@date Mayo, 2013
*/

#include "AmplifyDamageFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CAmplifyDamageFeedback);

	//__________________________________________________________________

	CAmplifyDamageFeedback::CAmplifyDamageFeedback() : ISpellFeedback("amplifyDamage") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CAmplifyDamageFeedback::~CAmplifyDamageFeedback() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CAmplifyDamageFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	} // spawn
	//__________________________________________________________________

	void CAmplifyDamageFeedback::spell(){
		// Efecto cuando se activa la gravedad

	} // spell
	//__________________________________________________________________
		
	void CAmplifyDamageFeedback::stopSpell(){
		// Efecto cuando se desactiva la gravedad

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic