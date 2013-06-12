/**
@file ShotGun.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CShotGunFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "ShotGunFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CShotGunFeedback);

	//__________________________________________________________________

	CShotGunFeedback::CShotGunFeedback() : IWeaponFeedback("shotGun") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CShotGunFeedback::~CShotGunFeedback() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CShotGunFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	}

	//__________________________________________________________________

	void CShotGunFeedback::primaryFire() {
		// Poner la animacion de carga inestable del arma
		_hudWeapon->shootAnim(-1.0f);
	}

	//__________________________________________________________________

	void CShotGunFeedback::stopPrimaryFire() {
		// Parar la animacion de carga
	}

	//__________________________________________________________________

	void CShotGunFeedback::secondaryFire() {
		// Hacer animacion de atraccion de balas
	}

}//namespace Logic