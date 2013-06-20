/**
@file SoulReaper.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CSoulReaperAmmo
@see Logic::IWeaponAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "SoulReaperAmmo.h"
#include "FireBallController.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "SoulReaper.h"
#include "SoulReaperFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CSoulReaperAmmo);

	//__________________________________________________________________

	CSoulReaperAmmo::CSoulReaperAmmo() : IWeaponAmmo("soulReaper"),
											_primaryFireIsActive(false),
											_secondaryFireIsActive(false),
											_primaryFireCooldownTimer(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CSoulReaperAmmo::~CSoulReaperAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CSoulReaperAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponAmmo::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireCooldown") );

		// Cooldown del disparo principal
		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireCooldown") * 1000;

		_friend[_friends] = _entity->getComponent<Logic::CSoulReaper>("CSoulReaper");
		if(_friend[_friends])
			++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CSoulReaperFeedback>("CSoulReaperFeedback");
		if(_friend[_friends])
			++_friends;
		if(_friends == 0)
			assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}

	//__________________________________________________________________

	void CSoulReaperAmmo::onActivate() {

	}

	//__________________________________________________________________

	void CSoulReaperAmmo::onAvailable() {
		IWeaponAmmo::onAvailable();
		
	}

	//__________________________________________________________________

	void CSoulReaperAmmo::onTick(unsigned int msecs) {
		
		// Controlamos el cooldown
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0){
				_primaryFireCooldownTimer = 0;
			}
		}
	}

	//__________________________________________________________________

	bool CSoulReaperAmmo::canUsePrimaryFire() {
		return _primaryFireCooldownTimer == 0 && !_secondaryFireIsActive;
	}

	//__________________________________________________________________

	bool CSoulReaperAmmo::canUseSecondaryFire() {
		return true;
	}

	//__________________________________________________________________

	void CSoulReaperAmmo::primaryFire() {
		IWeaponAmmo::primaryFire();

		_primaryFireCooldownTimer = _primaryFireCooldown;

		_primaryFireIsActive = true;

	} // primaryFire
	//__________________________________________________________________

	void CSoulReaperAmmo::stopPrimaryFire() {
		IWeaponAmmo::stopPrimaryFire();
		
		_primaryFireIsActive = false;
	} // stopPrimaryFire
	//__________________________________________________________________

	void CSoulReaperAmmo::secondaryFire() {
		IWeaponAmmo::secondaryFire();

		_secondaryFireIsActive = true;

	} // secondaryFire
	//__________________________________________________________________

	void CSoulReaperAmmo::stopSecondaryFire() {
		if(!_secondaryFireIsActive) return;

		IWeaponAmmo::stopSecondaryFire();
		
		_secondaryFireIsActive = false;
	} // stopPrimaryFire
	//__________________________________________________________________

	void CSoulReaperAmmo::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown - (percentage * _primaryFireCooldown * 0.01f));
	}

}//namespace Logic