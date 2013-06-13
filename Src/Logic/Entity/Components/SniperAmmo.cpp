/**
@file Sniper.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CSniperAmmo
@see Logic::IWeaponAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "SniperAmmo.h"
#include "FireBallController.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Sniper.h"
#include "SniperFeedback.h"
using namespace std;

namespace Logic {

	IMP_FACTORY(CSniperAmmo);

	//__________________________________________________________________

	CSniperAmmo::CSniperAmmo() : IWeaponAmmo("sniper"),
								_primaryFireCooldown(0),
								_defaultPrimaryFireCooldown(0),
								_primaryFireCooldownTimer(0),
								_primaryFireIsActive(false),
								_secondaryFireCooldown(0),
								_defaultSecondaryFireCooldown(0),
								_secondaryFireCooldownTimer(0),
								_secondaryFireIsActive(false),
								_ammoSpentPerSecondaryShot(0){
		
	}

	//__________________________________________________________________

	CSniperAmmo::~CSniperAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CSniperAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponAmmo::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireCooldown") );

		assert( entityInfo->hasAttribute(_weaponName + "SecondaryFireCooldown") );
		assert( entityInfo->hasAttribute(_weaponName + "AmmoSpentPerSecondaryShot") );

		// Cooldown del disparo principal
		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireCooldown") * 1000;
		_defaultSecondaryFireCooldown = _secondaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "SecondaryFireCooldown") * 1000;

		// Ratio al que gastamos municion
		_ammoSpentPerSecondaryShot = entityInfo->getIntAttribute(_weaponName + "AmmoSpentPerSecondaryShot");

		_friend = _entity->getComponent<Logic::CSniper>("CSniper");
		if(!_friend)
			_friend = _entity->getComponent<Logic::CSniperFeedback>("CSniperFeedback");

		return true;
	}

	//__________________________________________________________________

	void CSniperAmmo::onActivate() {

	}

	//__________________________________________________________________

	void CSniperAmmo::onAvailable() {
		IWeaponAmmo::onAvailable();

	}

	//__________________________________________________________________

	void CSniperAmmo::onTick(unsigned int msecs) {
		
		// tengo el cooldown compartio entre ambos disparos.
		// Controlamos el cooldown
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0){
				_primaryFireCooldownTimer = 0;
			}
		}
	}

	//__________________________________________________________________

	bool CSniperAmmo::canUsePrimaryFire() {
		return _primaryFireCooldownTimer == 0 && _currentAmmo > 0 && !_secondaryFireIsActive;
	}

	//__________________________________________________________________

	bool CSniperAmmo::canUseSecondaryFire() {
		return _secondaryFireCooldownTimer == 0 && _currentAmmo >= _ammoSpentPerSecondaryShot && !_primaryFireIsActive;
	}

	//__________________________________________________________________

	void CSniperAmmo::primaryFire() {
		IWeaponAmmo::primaryFire();

		_primaryFireCooldownTimer = _primaryFireCooldown;

		_primaryFireIsActive = true;
		decrementAmmo();
		
	}

	//__________________________________________________________________

	void CSniperAmmo::stopPrimaryFire() {
		IWeaponAmmo::stopPrimaryFire();
		
		_primaryFireIsActive = false;
	}

	//__________________________________________________________________

	void CSniperAmmo::secondaryFire() {
		IWeaponAmmo::secondaryFire();

		_primaryFireCooldownTimer = _secondaryFireCooldown;
		_secondaryFireIsActive = false;

		for(unsigned int i = 0; i < _ammoSpentPerSecondaryShot; ++i)
			decrementAmmo();
	}

	//__________________________________________________________________

	void CSniperAmmo::stopSecondaryFire() {
		IWeaponAmmo::stopSecondaryFire();

		_secondaryFireIsActive = false;
	}
	//__________________________________________________________________

	void CSniperAmmo::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown - (percentage * _primaryFireCooldown * 0.01f));

		_secondaryFireCooldown = percentage == 0 ? _defaultSecondaryFireCooldown : (_defaultSecondaryFireCooldown - (percentage * _secondaryFireCooldown * 0.01f));
	}

}//namespace Logic