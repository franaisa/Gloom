/**
@file MiniGun.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CMiniGunAmmo
@see Logic::IWeaponAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "MiniGunAmmo.h"
#include "FireBallController.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "MiniGun.h"
#include "MiniGunFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CMiniGunAmmo);

	//__________________________________________________________________

	CMiniGunAmmo::CMiniGunAmmo() : IWeaponAmmo("miniGun"),
									_primaryFireCooldown(0),
									_defaultPrimaryFireCooldown(0),
									_primaryFireCooldownTimer(0),
									_primaryFireIsActive(false),
									_secondaryFireCooldown(0),
									_defaultSecondaryFireCooldown(0),
									_secondaryFireCooldownTimer(0),
									_secondaryFireIsActive(false),
									_ammoSpentTimer(0),
									_ammoSpentTimeStep(0),
									_defaultAmmoSpentTimeStep(0),
									_currentSpentSecondaryAmmo(0),
									_maxAmmoSpentPerSecondaryShot(0),
									_secondaryFireLoadTime(0){
		
	}

	//__________________________________________________________________

	CMiniGunAmmo::~CMiniGunAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CMiniGunAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponAmmo::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireCooldown") );
		assert( entityInfo->hasAttribute(_weaponName + "SecondaryFireCooldown") );
		assert( entityInfo->hasAttribute(_weaponName + "SecondaryFireLoadTime") );
		assert( entityInfo->hasAttribute(_weaponName + "MaxAmmoSpentPerSecondaryShot") );

		// Cooldown del disparo principal
		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireCooldown") * 1000;

		// Cooldown del disparo secundario
		_defaultSecondaryFireCooldown = _secondaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "SecondaryFireCooldown") * 1000;

		// Tiempo de carga del arma
		_secondaryFireLoadTime = entityInfo->getFloatAttribute(_weaponName + "SecondaryFireLoadTime") * 1000;

		// Ratio al que gastamos municion
		_maxAmmoSpentPerSecondaryShot = entityInfo->getIntAttribute(_weaponName + "MaxAmmoSpentPerSecondaryShot");
		_defaultAmmoSpentTimeStep = _ammoSpentTimeStep = (float)_secondaryFireLoadTime / (float)(_maxAmmoSpentPerSecondaryShot);

		_friend[_friends] = _entity->getComponent<Logic::CMiniGun>("CMiniGun");
		if(_friend[_friends]) ++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CMiniGunFeedback>("CMiniGunFeedback");
		if(_friend[_friends]) ++_friends;
		if(_friends == 0) assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}

	//__________________________________________________________________

	void CMiniGunAmmo::onActivate() {
		
	}

	//__________________________________________________________________

	void CMiniGunAmmo::onAvailable() {
		IWeaponAmmo::onAvailable();
		
	}

	//__________________________________________________________________

	void CMiniGunAmmo::onFixedTick(unsigned int msecs) {		
		
		// Controlamos el cooldown
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0){
				_primaryFireCooldownTimer = 0;
					if(_primaryFireIsActive){
						if(canUsePrimaryFire()){
						// las alternativas son, o enviar un mensaje por cada disparo, o q el componente de shoot tb tenga el cooldown
							decrementAmmo();
							_primaryFireCooldownTimer = _primaryFireCooldown;
						}else{
							stopPrimaryFire();
						}
				}
			}
		}

		if(_secondaryFireIsActive) {
			if(_currentAmmo > 0 && _currentSpentSecondaryAmmo < _maxAmmoSpentPerSecondaryShot) {
				// Actualizamos el timer que se encarga de reducir la municion
				_ammoSpentTimer += msecs;
				if(_ammoSpentTimer >= _ammoSpentTimeStep) {
					decrementAmmo();
					++_currentSpentSecondaryAmmo;
					_ammoSpentTimer = 0;
				}
			}
		}
		
		// Controlamos el cooldown
		if(_secondaryFireCooldownTimer > 0) {
			_secondaryFireCooldownTimer -= msecs;
			
			if(_secondaryFireCooldownTimer < 0)
				_secondaryFireCooldownTimer = 0;
		}
	}

	//__________________________________________________________________

	bool CMiniGunAmmo::canUsePrimaryFire() {
		return _primaryFireCooldownTimer == 0 && _currentAmmo > 0 && !_secondaryFireIsActive;
	}

	//__________________________________________________________________

	bool CMiniGunAmmo::canUseSecondaryFire() {
		return _secondaryFireCooldownTimer == 0 && _currentAmmo > 0 && !_primaryFireIsActive;
	}

	//__________________________________________________________________

	void CMiniGunAmmo::primaryFire() {
		IWeaponAmmo::primaryFire();

		_primaryFireIsActive = true;

		_primaryFireCooldownTimer = _primaryFireCooldown;
		decrementAmmo();
		
	}
	//__________________________________________________________________

	void CMiniGunAmmo::stopPrimaryFire() {
		IWeaponAmmo::stopPrimaryFire();

		_primaryFireIsActive = false;
	}
	//__________________________________________________________________

		void CMiniGunAmmo::secondaryFire() {
		IWeaponAmmo::secondaryFire();

		_secondaryFireCooldownTimer = _secondaryFireCooldown;

		_secondaryFireIsActive = true;
		// la 1º bala la debo de quitar aqui, si no esperara el cold down y disparara gratis
		decrementAmmo();
		++_currentSpentSecondaryAmmo;
	}
	//__________________________________________________________________

	void CMiniGunAmmo::stopSecondaryFire() {
		IWeaponAmmo::stopSecondaryFire();

		_secondaryFireIsActive = false;

		// Reseteamos el reloj
		_currentSpentSecondaryAmmo = _ammoSpentTimer = 0;
	}
	//__________________________________________________________________

	void CMiniGunAmmo::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown - (percentage * _primaryFireCooldown * 0.01f));

		_secondaryFireCooldown = percentage == 0 ? _defaultSecondaryFireCooldown : (_defaultSecondaryFireCooldown - (percentage * _secondaryFireCooldown * 0.01f));

		_ammoSpentTimeStep = percentage == 0 ? _defaultAmmoSpentTimeStep : (_defaultAmmoSpentTimeStep - (percentage * _ammoSpentTimeStep * 0.01f));
		
	}

}//namespace Logic