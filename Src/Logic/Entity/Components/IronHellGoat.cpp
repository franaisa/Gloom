/**
@file IronHellGoat.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CIronHellGoat
@see Logic::CShootProjectile

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "IronHellGoat.h"
#include "DynamicLight.h"
#include "FireBallController.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CIronHellGoat);

	//__________________________________________________________________

	CIronHellGoat::CIronHellGoat() : IWeapon("ironHellGoat"),
									 _primaryFireIsActive(false),
									 _secondaryFireIsActive(false),
									 _elapsedTime(0),
									 _ammoSpentTimer(0),
									 _currentSpentAmmo(0),
									 _primaryFireCooldownTimer(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CIronHellGoat::~CIronHellGoat() {
		if( !_controllableFireBalls.empty() ) {
			for(auto it = _controllableFireBalls.begin(); it != _controllableFireBalls.end(); ++it) {
				(*it)->setOwner(NULL);
			}
		}
	}

	//__________________________________________________________________

	bool CIronHellGoat::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		Map::CEntity* weapon = CEntityFactory::getSingletonPtr()->getInfo(_weaponName);

		if( !IWeapon::spawn(entity, map, weapon) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( weapon->hasAttribute( "PrimaryFireCooldown") );
		assert( weapon->hasAttribute( "MaximumLoadingTime") );
		assert( weapon->hasAttribute( "DefaultFireBallRadius") );
		assert( weapon->hasAttribute( "DefaultFireBallSpeed" ) );
		assert( weapon->hasAttribute( "DefaultFireBallExplotionRadius") );
		assert( weapon->hasAttribute( "DefaultFireBallDamage") );
		assert( weapon->hasAttribute( "MaxAmmoPerShot") );
		assert( weapon->hasAttribute( "MaxFireBallRadius") );
		assert( weapon->hasAttribute( "MaxFireBallSpeed") );
		assert( weapon->hasAttribute( "MaxFireBallExplotionRadius") );
		assert( weapon->hasAttribute( "MaxFireBallDamage") );
		assert( weapon->hasAttribute( "ExplotionStrength") );
		assert( weapon->hasAttribute( "Audio") );

		// Cooldown del disparo principal
		_defaultPrimaryFireCooldown = _primaryFireCooldown = weapon->getFloatAttribute( "PrimaryFireCooldown") * 1000;

		// Tiempo de carga del arma
		_maxLoadingTime = weapon->getFloatAttribute( "MaximumLoadingTime") * 1000;

		// Ratio al que gastamos municion
		_maxAmmoPerShot = weapon->getIntAttribute( "MaxAmmoPerShot");
		_ammoSpentTimeStep = (float)_maxLoadingTime / (float)(_maxAmmoPerShot);

		// Valores de creación de la bola de fuego por defecto
		_defaultFireBallRadius = weapon->getFloatAttribute( "DefaultFireBallRadius");
		_defaultFireBallSpeed = weapon->getFloatAttribute( "DefaultFireBallSpeed");
		_defaultFireBallExplotionRadius = weapon->getFloatAttribute( "DefaultFireBallExplotionRadius");
		_currentDefaultFireBallDamage = _defaultFireBallDamage = weapon->getFloatAttribute( "DefaultFireBallDamage");

		// Valores de creación máximos de la bola de fuego
		float maxFireBallRadius = weapon->getFloatAttribute( "MaxFireBallRadius");
		float maxFireBallSpeed = weapon->getFloatAttribute( "MaxFireBallSpeed");
		float maxFireBallExplotionRadius = weapon->getFloatAttribute( "MaxFireBallExplotionRadius");
		_currentMaxFireBallDamage = _maxFireBallDamage = weapon->getFloatAttribute( "MaxFireBallDamage");

		// Calculamos los factores de crecimiento en funcion del tiempo maximo de carga
		_fireBallRadiusTemporalIncrement = (maxFireBallRadius - _defaultFireBallRadius) / _maxLoadingTime;
		_fireBallSpeedTemporalIncrement = (maxFireBallSpeed - _defaultFireBallSpeed) / _maxLoadingTime;
		_fireBallExplotionRadiusTemporalIncrement = (maxFireBallExplotionRadius - _defaultFireBallExplotionRadius) / _maxLoadingTime;

		_explotionStrength = weapon->getFloatAttribute("ExplotionStrength");
		// Obtenemos los sonidos que produce el arma
		_shootAudio = weapon->getStringAttribute("Audio");

		return true;
	}

	//__________________________________________________________________

	void CIronHellGoat::onActivate() {
		_currentSpentAmmo = _ammoSpentTimer = _elapsedTime = 0;
		
		/*
		//Reiniciamos el cooldown
		reduceCooldown(0);
		//y los daños
		amplifyDamage(0);
		*/
	}

	//__________________________________________________________________

	void CIronHellGoat::onAvailable() {
		IWeapon::onAvailable();
		_currentSpentAmmo = _ammoSpentTimer = _elapsedTime = 0;
	}

	//__________________________________________________________________

	void CIronHellGoat::onTick(unsigned int msecs) {
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
		else if(_secondaryFireIsActive) {
			// Si hay bolas vivas, mover las en función de la velocidad que
			// llevan y la dirección en la que apunto con el ratón
			for(auto it = _controllableFireBalls.begin(); it != _controllableFireBalls.end(); ++it) {
				(*it)->alterDirection();
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

	void CIronHellGoat::primaryFire() {
		_primaryFireIsActive = true;
		_primaryFireCooldownTimer = _primaryFireCooldown;

		decrementAmmo();
		++_currentSpentAmmo;
	}

	//__________________________________________________________________

	void CIronHellGoat::stopPrimaryFire() {
		if(!_primaryFireIsActive) return;

		_primaryFireIsActive = false;

		createFireBall();

		// Reseteamos el reloj
		_currentSpentAmmo = _ammoSpentTimer = _elapsedTime = 0;
	}

	//__________________________________________________________________

	void CIronHellGoat::secondaryFire() {
		_secondaryFireIsActive = true;
	}

	//__________________________________________________________________

	void CIronHellGoat::stopSecondaryFire() {
		_secondaryFireIsActive = false;
	}

	//__________________________________________________________________

	void CIronHellGoat::removeFireBall(CFireBallController* fireBall) {
		// Borrar esta bola de la lista de bolas que podemos controlar.
		_controllableFireBalls.erase(fireBall);
	}

	//__________________________________________________________________

	void CIronHellGoat::resetAmmo() {
		// Ponemos la munición a 0
		IWeapon::resetAmmo();
		// Limpiamos la lista de bolas controlables
		if( !_controllableFireBalls.empty() ) {
			for(auto it = _controllableFireBalls.begin(); it != _controllableFireBalls.end(); ++it) {
				(*it)->setOwner(NULL);
			}
			_controllableFireBalls.clear();
		}
	}

	//__________________________________________________________________

	void CIronHellGoat::amplifyDamage(unsigned int percentage) {

		// Si es 0 significa que hay que restaurar al que habia por defecto
		if(percentage == 0) {
			_currentDefaultFireBallDamage = _defaultFireBallDamage;
			_currentMaxFireBallDamage = _maxFireBallDamage;
		}
		// Sino aplicamos el porcentaje pasado por parámetro
		else {
			_currentDefaultFireBallDamage += percentage * _currentDefaultFireBallDamage * 0.01f;
			_currentMaxFireBallDamage += percentage * _currentMaxFireBallDamage * 0.01f;
		}
	}

	//__________________________________________________________________

	void CIronHellGoat::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown - (percentage * _primaryFireCooldown * 0.01f));
	}

	//__________________________________________________________________

	void CIronHellGoat::createFireBall() {
		// Obtenemos la información estandard asociada a la bola de fuego
		Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("FireBall")->clone();

		// Calculamos los valores customizados para la creacion de la bola de fuego
		// en funcion del tiempo que hemos mantenido pulsado el disparo primario
		float fireBallRadius = _defaultFireBallRadius + (_fireBallRadiusTemporalIncrement * _elapsedTime);
		float fireBallSpeed = _defaultFireBallSpeed + (_fireBallSpeedTemporalIncrement * _elapsedTime);
		float fireBallExplotionRadius = _defaultFireBallExplotionRadius + (_fireBallExplotionRadiusTemporalIncrement * _elapsedTime);
		float fireBallDamage = _currentDefaultFireBallDamage + (((_currentMaxFireBallDamage - _currentDefaultFireBallDamage) / _maxLoadingTime) * _elapsedTime);

		// Modificamos sus parámetros en base a los valores calculados
		entityInfo->setAttribute( "physic_radius", toString(fireBallRadius) );
		entityInfo->setAttribute( "scale", toString( Vector3(1.0f, 1.0f, 1.0f) * (fireBallRadius / _defaultFireBallRadius) ) );
		entityInfo->setAttribute( "speed", toString(fireBallSpeed) );
		entityInfo->setAttribute( "explotionRadius", toString(fireBallExplotionRadius) );
		entityInfo->setAttribute( "damage", toString(fireBallDamage) );
		entityInfo->setAttribute( "direction", toString( _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z ));
		entityInfo->setAttribute( "strength", toString( _explotionStrength + _elapsedTime/ _maxLoadingTime ) );
		

		// El único atributo que nos interesa pasar al cliente es la escala para que la bola
		// aparezca gráficamente más grande o más pequeña
		Map::CEntity* clientEntityInfo = new Map::CEntity( entityInfo->getName() );
		clientEntityInfo->setAttribute( "scale", toString( Vector3(1.0f, 1.0f, 1.0f) * (fireBallRadius / _defaultFireBallRadius) ) );

		// Creamos la bola de fuego con los parámetros customizados a partir
		// del radio de la cápsula + el radio de la bola + un pequeño offset
		Vector3 shootPosition = _entity->getPosition() + (( _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z ) * (_capsuleRadius + fireBallRadius + 0.5f) );
		shootPosition.y += _heightShoot - fireBallRadius;

		// Creamos la entidad
		CEntity* fireBall = CEntityFactory::getSingletonPtr()->createCustomClientEntity(
								entityInfo,
								clientEntityInfo,
								CServer::getSingletonPtr()->getMap(),
								shootPosition,
								_entity->getOrientation()
							);

		// Eliminamos la copia de la información que hemos creado
		delete entityInfo;
		// Eliminamos la información adicional para el cliente
		delete clientEntityInfo;

		// Le indicamos al controlador de la bola que este componente es el poseedor
		// para que se invoque al metodo correspondiente cuando las bolas mueran
		CFireBallController* fbController = fireBall->getComponent<CFireBallController>("CFireBallController");
		fbController->setOwner(this);

		// Arrancamos la entidad
		fireBall->activate();
		fireBall->start();

		// Me apunto la entidad devuelta por la factoria
		_controllableFireBalls.insert(fbController);
	}

}//namespace Logic