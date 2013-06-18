/**
@file ComeBackAmmo.cpp

Contiene la implementacion del hechizo de regreso.

@see Logic::CComeBackAmmo
@see Logic::ISpellAmmo

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/



#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "ComeBackAmmo.h"
#include "ComeBack.h"
#include "ComeBackFeedback.h"

#include "Logic/Messages/MessageReducedCooldown.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CComeBackAmmo);

	//__________________________________________________________________

	CComeBackAmmo::CComeBackAmmo() : ISpellAmmo("comeBack"),
								_spellIsActive(false),
								_defaultCooldown(0),
								_duration(0),
								_maxAmmo(0),
								_ammoPerPull(0), 
								_distanceComeBack(0){
		// Nada que hacer
	}

	//__________________________________________________________________

	CComeBackAmmo::~CComeBackAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CComeBackAmmo::accept(const shared_ptr<CMessage>& message) {
		return ISpellAmmo::accept(message) || 
			message->getMessageType() == Message::REDUCED_COOLDOWN;
	} // 
	//__________________________________________________________________

	void CComeBackAmmo::process(const shared_ptr<CMessage>& message) {
		ISpellAmmo::process(message);
		switch( message->getMessageType() ) {
			case Message::REDUCED_COOLDOWN: {
				reduceCooldown(static_pointer_cast<CMessageReducedCooldown>(message)->getPercentCooldown());
			break;
			}
		}
	} // process
	//__________________________________________________________________
	bool CComeBackAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellAmmo::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_spellName + "Cooldown") );
		assert( entityInfo->hasAttribute(_spellName + "MaxAmmo") );
		assert( entityInfo->hasAttribute(_spellName + "AmmoPerPull") );
		assert( entityInfo->hasAttribute(_spellName + "DurationEffect") );
		assert( entityInfo->hasAttribute(_spellName + "DistanceEffect") );

		// Cooldown del disparo principal
		_defaultCooldown = _cooldown = entityInfo->getFloatAttribute(_spellName + "Cooldown") * 1000;
		_duration = entityInfo->getFloatAttribute(_spellName + "DurationEffect") * 1000;
		assert( _defaultCooldown > _duration && "Cuidado que el coolDown es menor que la duracion del hechizo");

		_maxAmmo = entityInfo->getIntAttribute(_spellName + "MaxAmmo");
		_ammoPerPull = entityInfo->getIntAttribute(_spellName + "AmmoPerPull");

		_distanceComeBack = entityInfo->getIntAttribute(_spellName + "DistanceEffect");

		_friend[_friends] = _entity->getComponent<Logic::CComeBack>("CComeBack");
		if(_friend[_friends]) ++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CComeBackFeedback>("CComeBackFeedback");
		if(_friend[_friends]) ++_friends;
		if(_friends == 0) assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}

	//__________________________________________________________________

	void CComeBackAmmo::onActivate() {
		// Aqui enviaria el mensaje o lo que fuera para que pusiera en el hud
	}

	//__________________________________________________________________

	void CComeBackAmmo::onAvailable() {
		ISpellAmmo::onAvailable();		
		_currentAmmo += _ammoPerPull;
		_currentAmmo = _currentAmmo > _maxAmmo ? _maxAmmo : _currentAmmo;
	}

	//__________________________________________________________________

	void CComeBackAmmo::onTick(unsigned int msecs) {
		
		// Controlamos el cooldown
		if(_cooldownTimer > 0) {
			_cooldownTimer -= msecs;
			
			if(_cooldownTimer < 0){
				_cooldownTimer = 0;
			}
		}
		if(_durationTimer > 0){
			_durationTimer -= msecs;
			if(_durationTimer < 0){
				// ya lo pongo a cero dentro del metodo
				stopSpell();
			}
		}
	}

	//__________________________________________________________________

	bool CComeBackAmmo::canUseSpell() {
		return _cooldownTimer == 0 && _currentAmmo > 0;
	}

	//__________________________________________________________________

	void CComeBackAmmo::spell() {
		ISpellAmmo::spell();
		// Si ya se esta haciendo el hechizo, significa que queremos pararlo
		if(!_spellIsActive ){
			--_currentAmmo;
			_cooldownTimer = _cooldown;
			_durationTimer = _duration;
			_spellIsActive = true;

			_comeBackPosition = _entity->getPosition();
			_comeBackOrientation = _entity->getOrientation();
		}else{
			if( _comeBackPosition.distance(_entity->getPosition()) <= _distanceComeBack){
				ISpellAmmo::spell();
				stopSpell();
			}
		}
		
	} // primaryFire
	//__________________________________________________________________

	void CComeBackAmmo::stopSpell() {
		ISpellAmmo::stopSpell();
		
		// Voy a beneficiar si se hace durante poco tiempo
		// con esto reduzco el cooldown el mismo porcentaje que me quedaba.
		//_cooldown *=(1-(_durationTimer / _duration ));
		_cooldown = 0;

		_durationTimer = 0;
		
		_comeBackPosition = Vector3::ZERO;
		_comeBackOrientation = Quaternion::ZERO;
		
		_spellIsActive = false;
	} // stopPrimaryFire
	//__________________________________________________________________

	
	void CComeBackAmmo::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		
		
		_cooldown = percentage == 0 ? _defaultCooldown : (_defaultCooldown - (percentage * _cooldown * 0.01f));
		assert(_cooldown < _duration && "La duracion del cooldown reducido es inferior a la del hechizo, lo cual no tiene mucho sentido");
		
	}

}//namespace Logic