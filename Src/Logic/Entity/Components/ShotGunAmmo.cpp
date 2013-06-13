/**
@file ShotGun.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CShotGunAmmo
@see Logic::IWeaponAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "ShotGunAmmo.h"
#include "FireBallController.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "ShotGun.h"
#include "ShotGunFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CShotGunAmmo);

	//__________________________________________________________________

	CShotGunAmmo::CShotGunAmmo() : IWeaponAmmo("shotGun"),
									_primaryFireCooldown(0),
									_defaultPrimaryFireCooldown(0),
									_primaryFireCooldownTimer(0),
									_primaryFireDispersion(0),
									_numberOfShots(0),
									_shotGunComponent(0){
		// Nada que hacer
	}

	//__________________________________________________________________

	CShotGunAmmo::~CShotGunAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CShotGunAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponAmmo::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireCooldown") );
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireDispersion") );
		assert( entityInfo->hasAttribute(_weaponName + "NumberOfShots") );
		

		// Cooldown del disparo principal
		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireCooldown") * 1000;

		_primaryFireDispersion = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireDispersion");
		_numberOfShots = entityInfo->getIntAttribute(_weaponName + "NumberOfShots");
		

		_friend = _entity->getComponent<Logic::CShotGun>("CShotGun");
		if(!_friend)
			_friend = _entity->getComponent<Logic::CShotGunFeedback>("CShotGunFeedback");
		else
			_shotGunComponent = _entity->getComponent<Logic::CShotGun>("CShotGun");;
		return true;
	}

	//__________________________________________________________________

	void CShotGunAmmo::onActivate() {
		
	}

	//__________________________________________________________________

	void CShotGunAmmo::onAvailable() {
		IWeaponAmmo::onAvailable();
		
	}

	//__________________________________________________________________

	void CShotGunAmmo::onTick(unsigned int msecs) {
	
		// Controlamos el cooldown
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0)
				_primaryFireCooldownTimer = 0;
		}
	}

	//__________________________________________________________________

	bool CShotGunAmmo::canUsePrimaryFire() {
		return _primaryFireCooldownTimer == 0 && _currentAmmo > 0;
	}

	//__________________________________________________________________

	bool CShotGunAmmo::canUseSecondaryFire() {
		return true;
	}

	//__________________________________________________________________

	void CShotGunAmmo::primaryFire() {
		IWeaponAmmo::primaryFire();

		_primaryFireCooldownTimer = _primaryFireCooldown;

		decrementAmmo();
	}

	//__________________________________________________________________

	void CShotGunAmmo::stopPrimaryFire() {
		IWeaponAmmo::stopPrimaryFire();
	}

	//__________________________________________________________________

	void CShotGunAmmo::addAmmo(int weapon, int ammo, bool iAmCatch){
		IWeaponAmmo::addAmmo(weapon, ammo, iAmCatch);

		if(_shotGunComponent)
			_shotGunComponent->setCurrentAmmo(_currentAmmo);
	}

	void CShotGunAmmo::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown - (percentage * _primaryFireCooldown * 0.01f));
	}

}//namespace Logic