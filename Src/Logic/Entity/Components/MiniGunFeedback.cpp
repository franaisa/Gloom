/**
@file MiniGun.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CMiniGunFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "MiniGunFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CMiniGunFeedback);

	//__________________________________________________________________

	CMiniGunFeedback::CMiniGunFeedback() : IWeaponFeedback("miniGun") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMiniGunFeedback::~CMiniGunFeedback() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CMiniGunFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	}

	//__________________________________________________________________

	void CMiniGunFeedback::primaryFire() {
		// Poner la animacion de carga inestable del arma
		_hudWeapon->continouosShooting(true);
	}

	//__________________________________________________________________

	void CMiniGunFeedback::stopPrimaryFire() {
		// Parar la animacion de carga
		_hudWeapon->continouosShooting(false);
	}

	//__________________________________________________________________

	void CMiniGunFeedback::secondaryFire() {
		_hudWeapon->loadingWeapon(true);
	}
	//__________________________________________________________________
	
	void CMiniGunFeedback::stopSecondaryFire() {
		// Parar la animacion de carga
		_hudWeapon->loadingWeapon(false);
		_hudWeapon->shootAnim(-1.85f);
	}

	//__________________________________________________________________
}//namespace Logic