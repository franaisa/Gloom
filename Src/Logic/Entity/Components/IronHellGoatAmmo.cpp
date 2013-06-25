/**
@file IronHellGoat.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CIronHellGoatAmmo
@see Logic::IWeaponAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "IronHellGoatAmmo.h"
#include "FireBallController.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "IronHellGoat.h"
#include "IronHellGoatFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CIronHellGoatAmmo);

	//__________________________________________________________________

	CIronHellGoatAmmo::CIronHellGoatAmmo() : IWeaponAmmo("ironHellGoat"),
											_primaryFireIsActive(false),
											_elapsedTime(0),
											_ammoSpentTimer(0),
											_currentSpentAmmo(0),
											_primaryFireCooldownTimer(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CIronHellGoatAmmo::~CIronHellGoatAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CIronHellGoatAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponAmmo::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireCooldown") );
		assert( entityInfo->hasAttribute(_weaponName + "MaximumLoadingTime") );
		assert( entityInfo->hasAttribute(_weaponName + "MaxAmmoPerShot") );

		// Cooldown del disparo principal
		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireCooldown") * 1000;

		// Tiempo de carga del arma
		_maxLoadingTime = entityInfo->getFloatAttribute(_weaponName + "MaximumLoadingTime") * 1000;

		// Ratio al que gastamos municion
		_maxAmmoPerShot = entityInfo->getIntAttribute(_weaponName + "MaxAmmoPerShot");
		_ammoSpentTimeStep = (float)_maxLoadingTime / (float)(_maxAmmoPerShot);

		_friend[_friends] = _entity->getComponent<Logic::CIronHellGoat>("CIronHellGoat");
		if(_friend[_friends]) ++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CIronHellGoatFeedback>("CIronHellGoatFeedback");
		if(_friend[_friends]) ++_friends;
		if(_friends == 0) assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}

	//__________________________________________________________________

	void CIronHellGoatAmmo::onActivate() {
		//Reiniciamos el cooldown
		reduceCooldown(0);
		
	}

	//__________________________________________________________________

	void CIronHellGoatAmmo::onAvailable() {
		IWeaponAmmo::onAvailable();
		_currentSpentAmmo = _ammoSpentTimer = _elapsedTime = 0;
		_primaryFireCooldown = _defaultPrimaryFireCooldown;
	}

	//__________________________________________________________________

	void CIronHellGoatAmmo::onTick(unsigned int msecs) {
		// Si el jugador esta dejando pulsado el disparo primario, aumentamos
		// el tamaño de la bola y reducimos la velocidad hasta un limite
		if(_primaryFireIsActive) {
			if(_currentAmmo > 0 && _currentSpentAmmo < _maxAmmoPerShot) {
				if(_elapsedTime < _maxLoadingTime) {
					// Contamos el tiempo que hemos mantenido pulsado el raton
					_elapsedTime += msecs;
					// Actualizamos el timer que se encarga de reducir la municion
					_ammoSpentTimer += msecs;
					if(_ammoSpentTimer >= _ammoSpentTimeStep) {
						decrementAmmo();
						++_currentSpentAmmo;
						_ammoSpentTimer = 0;
					}

					if(_elapsedTime >= _maxLoadingTime) {
						_elapsedTime = _maxLoadingTime;
					}
				}
			}
		}
		
		// Controlamos el cooldown
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0)
				_primaryFireCooldownTimer = 0;
		}
	}

	//__________________________________________________________________

	bool CIronHellGoatAmmo::canUsePrimaryFire() {
		if(_currentAmmo==0)
			emitSound(_noAmmoSound, false, true, false);

		return _primaryFireCooldownTimer == 0 && _currentAmmo > 0;
	}

	//__________________________________________________________________

	bool CIronHellGoatAmmo::canUseSecondaryFire() {
		return true;
	}

	//__________________________________________________________________

	void CIronHellGoatAmmo::primaryFire() {
		IWeaponAmmo::primaryFire();

		_primaryFireCooldownTimer = _primaryFireCooldown;

		decrementAmmo();
		++_currentSpentAmmo;
		
		_primaryFireIsActive = true;
	}

	//__________________________________________________________________

	void CIronHellGoatAmmo::stopPrimaryFire() {
		if(!_primaryFireIsActive) return;
			
		IWeaponAmmo::stopPrimaryFire();
		
		_primaryFireIsActive = false;

		// Reseteamos el reloj
		_currentSpentAmmo = _ammoSpentTimer = _elapsedTime = 0;
	}

	//__________________________________________________________________

	void CIronHellGoatAmmo::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown - (percentage * _primaryFireCooldown * 0.01f));


	}

}//namespace Logic