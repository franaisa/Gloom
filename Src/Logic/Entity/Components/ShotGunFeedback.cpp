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
#include "DynamicLight.h"
#include "CameraFeedbackNotifier.h"

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
		// Animación de disparo
		_hudWeapon->shootAnim(-1.5f);
		// Sonido de disparo
		emitSound(_weaponSound, false, true, false, false);

		// Shader de onda
		CCameraFeedbackNotifier* _cameraFX = _entity->getComponent<CCameraFeedbackNotifier>("CCameraFeedbackNotifier");
		_cameraFX->weaponShockWave( Vector2(0.67f, 0.77f), 0.37f, 0.0009f, 0.04f, 40.0f, 0.3f );

		// Shoot flash
		CDynamicLight* shootFlash = _entity->getComponent<CDynamicLight>("CDynamicLight");
		shootFlash->setColor( Vector3(1.0f, 1.0f, 0.72f) );
		shootFlash->setAttenuation( Vector3(1.0f, 0.014f, 0.0007f) );
		shootFlash->setRange(325.0f);
		shootFlash->turnOn( Vector3(0.0f, _heightShoot, 0.0f), 0.1f );
	}

	//__________________________________________________________________

	void CShotGunFeedback::stopPrimaryFire() {
	}

	//__________________________________________________________________

	void CShotGunFeedback::secondaryFire() {
		// Hacer animacion de atraccion de balas
	}

}//namespace Logic