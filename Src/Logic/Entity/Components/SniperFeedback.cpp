/**
@file Sniper.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CSniperFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "SniperFeedback.h"
#include "HudWeapons.h"
#include "DynamicLight.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CSniperFeedback);

	//__________________________________________________________________

	CSniperFeedback::CSniperFeedback() : IWeaponFeedback("sniper") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CSniperFeedback::~CSniperFeedback() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CSniperFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	}

	//__________________________________________________________________

	void CSniperFeedback::primaryFire() {
		_hudWeapon->shootAnim(-1.5f);
		//Sonido
		emitSound(_weaponSound, false, true, false, false);

		// Shoot flash
		CDynamicLight* shootFlash = _entity->getComponent<CDynamicLight>("CDynamicLight");
		shootFlash->setColor( Vector3(0.04f, 0.91f, 0.97f) );
		shootFlash->setAttenuation( Vector3(1.0f, 0.014f, 0.0007f) );
		shootFlash->setRange(325.0f);
		shootFlash->turnOn( Vector3(0.0f, _heightShoot, 0.0f), 0.1f );
	}

	//__________________________________________________________________

	void CSniperFeedback::stopPrimaryFire() {
		
	}

	//__________________________________________________________________

	void CSniperFeedback::secondaryFire() {
		_hudWeapon->shootAnim(-1.5f);
		//Sonido
		emitSound(_weaponSound, false, true, false, false);
		// Shoot flash
		CDynamicLight* shootFlash = _entity->getComponent<CDynamicLight>("CDynamicLight");
		shootFlash->setColor( Vector3(0.04f, 0.91f, 0.97f) );
		shootFlash->setAttenuation( Vector3(1.0f, 0.014f, 0.0007f) );
		shootFlash->setRange(325.0f);
		shootFlash->turnOn( Vector3(0.0f, _heightShoot, 0.0f), 0.1f );
	}

}//namespace Logic