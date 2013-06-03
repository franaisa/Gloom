/**
@file SoulReaper.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CSoulReaperFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "SoulReaperFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CSoulReaperFeedback);

	//__________________________________________________________________

	CSoulReaperFeedback::CSoulReaperFeedback() : IWeaponFeedback("soulReaper") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CSoulReaperFeedback::~CSoulReaperFeedback() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CSoulReaperFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	}

	//__________________________________________________________________

	void CSoulReaperFeedback::primaryFire() {
		// Poner la animacion de carga inestable del arma
		_hudWeapon->loadingWeapon(true);
	}

	//__________________________________________________________________

	void CSoulReaperFeedback::stopPrimaryFire() {
		// Parar la animacion de carga
		_hudWeapon->loadingWeapon(false);
		_hudWeapon->shootAnim(-1.85f);
	}

	//__________________________________________________________________

	void CSoulReaperFeedback::secondaryFire() {
		// De momento ná pisha
	}

}//namespace Logic