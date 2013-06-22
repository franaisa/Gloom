/**
@file Shield.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CShieldAmmo
@see Logic::ISpellAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/



#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "ShieldAmmo.h"
#include "Shield.h"
#include "ShieldFeedback.h"

#include "Logic/Messages/MessageReducedCooldown.h"



using namespace std;

namespace Logic {

	IMP_FACTORY(CShieldAmmo);

	//__________________________________________________________________

	CShieldAmmo::CShieldAmmo() : ISpellAmmo("shield"),
								_defaultCooldown(0),
								_cooldown(0),
								_duration(0),
								_maxAmmo(0),
								_ammoPerPull(0),
								_currentAmmo(0),
								_cooldownTimer(0),
								_durationTimer(0){
		// Nada que hacer
	}

	//__________________________________________________________________

	CShieldAmmo::~CShieldAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CShieldAmmo::accept(const shared_ptr<CMessage>& message) {
		return ISpellAmmo::accept(message) || 
			message->getMessageType() == Message::REDUCED_COOLDOWN;
	} // 
	//__________________________________________________________________

	void CShieldAmmo::process(const shared_ptr<CMessage>& message) {
		ISpellAmmo::process(message);
		switch( message->getMessageType() ) {
			case Message::REDUCED_COOLDOWN: {
				reduceCooldown(static_pointer_cast<CMessageReducedCooldown>(message)->getPercentCooldown());
			break;
			}
		}
	} // process
	//__________________________________________________________________
	bool CShieldAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellAmmo::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_spellName + "Cooldown") );
		assert( entityInfo->hasAttribute(_spellName + "MaxAmmo") );
		assert( entityInfo->hasAttribute(_spellName + "AmmoPerPull") );
		assert( entityInfo->hasAttribute(_spellName + "DurationEffect") );

		// Cooldown del disparo principal
		_defaultCooldown = _cooldown = entityInfo->getFloatAttribute(_spellName + "Cooldown") * 1000;

		_duration = entityInfo->getFloatAttribute(_spellName + "DurationEffect") * 1000;
		assert( _defaultCooldown > _duration && "Cuidado que el coolDown es menor que la duracion del hechizo");

		_maxAmmo = entityInfo->getIntAttribute(_spellName + "MaxAmmo");
		_ammoPerPull = entityInfo->getIntAttribute(_spellName + "AmmoPerPull");

		_friend[_friends] = _entity->getComponent<Logic::CShield>("CShield");
		if(_friend[_friends]) ++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CShieldFeedback>("CShieldFeedback");
		if(_friend[_friends]) ++_friends;
		if(_friends == 0) assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}

	//__________________________________________________________________

	void CShieldAmmo::onActivate() {
		ISpellAmmo::onActivate();
		// Aqui enviaria el mensaje o lo que fuera para que pusiera en el hud
		
	}

	//__________________________________________________________________

	void CShieldAmmo::onWake() {
		ISpellAmmo::onWake();
		
		_currentAmmo += _ammoPerPull;
		_currentAmmo = _currentAmmo > _maxAmmo ? _maxAmmo : _currentAmmo;
	}

	//__________________________________________________________________

	void CShieldAmmo::onTick(unsigned int msecs) {
		
		//printf("\n_cooldown timer: %d", _cooldownTimer);
		//printf("\n_duration timer: %d", _durationTimer);
		// Controlamos el cooldown
		if(_cooldownTimer > 0) 
			_cooldownTimer -= msecs;
		if(_cooldownTimer < 0)
			_cooldownTimer = 0;
		
		if(_spellIsActive){
			if(_durationTimer > 0)
				_durationTimer -= msecs;
			if(_durationTimer <= 0)
				stopSpell(); // ya lo pongo a cero dentro del metodo
		}
	}

	//__________________________________________________________________

	bool CShieldAmmo::canUseSpell() {
		return _spellIsActive || (_cooldownTimer == 0 && _currentAmmo > 0);
	}

	//__________________________________________________________________

	void CShieldAmmo::spell() {
		ISpellAmmo::spell();

		// Si ya se esta haciendo el hechizo, significa que queremos pararlo
		if(!_spellIsActive){
			--_currentAmmo;
			_cooldownTimer = _cooldown;
			_durationTimer = _duration;
			_spellIsActive = true;
		}else{
			stopSpell();
		}

	} // primaryFire
	//__________________________________________________________________

	void CShieldAmmo::stopSpell() {
		ISpellAmmo::stopSpell();
		
		// Voy a beneficiar si se hace durante poco tiempo
		// con esto reduzco el cooldown el mismo porcentaje que me quedaba.
		_durationTimer = _durationTimer < 0 ? 0 : _durationTimer;
		_cooldownTimer *=(1-((float)_durationTimer / (float)_duration ));

		_durationTimer = 0;
		
		_spellIsActive = false;
	} // stopSpell
	//__________________________________________________________________

	void CShieldAmmo::addAmmo(){ 
			_currentAmmo += _ammoPerPull;
			if(_currentAmmo > _maxAmmo)
				_currentAmmo = _maxAmmo;
		//quizas aqui , habria que llamar al addAmmo de los friends, por si acaso estos tiene que hacer algo con el hud por ejemplo
	} // addAmmo
	//__________________________________________________________________

	void CShieldAmmo::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_cooldown = percentage == 0 ? _defaultCooldown : (_defaultCooldown - (percentage * _cooldown * 0.01f));
		if(_cooldown < _duration) _cooldown = _duration;
		assert(_cooldown >= _duration && "La duracion del cooldown reducido es inferior a la del hechizo, lo cual no tiene mucho sentido");
	}

}//namespace Logic