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

	CIronHellGoat::CIronHellGoat() : CShootProjectile("ironHellGoat"),
									 _primaryFireIsActive(false),
									 _secondaryFireIsActive(false),
									 _elapsedTime(0),
									 _ammoSpentTimer(0),
									 _currentSpentAmmo(0) {
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
		if( !CShootProjectile::spawn(entity, map, entityInfo) ) return false;

		// Leer los parametros que toquen para los proyectiles
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;
		std::string weaponName = aux.str();

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(weaponName + "MaximumLoadingTime") );
		assert( entityInfo->hasAttribute(weaponName + "DefaultFireBallRadius") );
		assert( entityInfo->hasAttribute(weaponName + "DefaultFireBallSpeed" ) );
		assert( entityInfo->hasAttribute(weaponName + "DefaultFireBallExplotionRadius") );
		assert( entityInfo->hasAttribute(weaponName + "DefaultFireBallDamage") );
		assert( entityInfo->hasAttribute(weaponName + "MaxAmmoPerShot") );
		assert( entityInfo->hasAttribute(weaponName + "MaxFireBallRadius") );
		assert( entityInfo->hasAttribute(weaponName + "MaxFireBallSpeed") );
		assert( entityInfo->hasAttribute(weaponName + "MaxFireBallExplotionRadius") );
		assert( entityInfo->hasAttribute(weaponName + "MaxFireBallDamage") );
		assert( entityInfo->hasAttribute(weaponName + "Audio") );

		// Tiempo de carga del arma
		_maxLoadingTime = entityInfo->getIntAttribute(weaponName + "MaximumLoadingTime") * 1000.0f;

		// Ratio al que gastamos municion
		_maxAmmoPerShot = entityInfo->getIntAttribute(weaponName + "MaxAmmoPerShot");
		_ammoSpentTimeStep = (float)_maxLoadingTime / (float)(_maxAmmoPerShot);
		
		// Valores de creación de la bola de fuego por defecto
		_defaultFireBallRadius = entityInfo->getFloatAttribute(weaponName + "DefaultFireBallRadius");
		_defaultFireBallSpeed = entityInfo->getFloatAttribute(weaponName + "DefaultFireBallSpeed");
		_defaultFireBallExplotionRadius = entityInfo->getFloatAttribute(weaponName + "DefaultFireBallExplotionRadius");
		_defaultFireBallDamage = entityInfo->getFloatAttribute(weaponName + "DefaultFireBallDamage");

		// Valores de creación máximos de la bola de fuego
		float maxFireBallRadius = entityInfo->getFloatAttribute(weaponName + "MaxFireBallRadius");
		float maxFireBallSpeed = entityInfo->getFloatAttribute(weaponName + "MaxFireBallSpeed");
		float maxFireBallExplotionRadius = entityInfo->getFloatAttribute(weaponName + "MaxFireBallExplotionRadius");
		float maxFireBallDamage = entityInfo->getFloatAttribute(weaponName + "MaxFireBallDamage");

		// Calculamos los factores de crecimiento en funcion del tiempo maximo de carga
		_fireBallRadiusTemporalIncrement = (maxFireBallRadius - _defaultFireBallRadius) / _maxLoadingTime;
		_fireBallSpeedTemporalIncrement = (maxFireBallSpeed - _defaultFireBallSpeed) / _maxLoadingTime;
		_fireBallExplotionRadiusTemporalIncrement = (maxFireBallExplotionRadius - _defaultFireBallExplotionRadius) / _maxLoadingTime;
		_fireBallDamageTemporalIncrement = (maxFireBallDamage - _defaultFireBallDamage) / _maxLoadingTime;

		// Obtenemos los sonidos que produce el arma
		_shootAudio = entityInfo->getStringAttribute(weaponName + "Audio");

		return true;
	}

	//__________________________________________________________________

	void CIronHellGoat::onActivate() {
		_currentSpentAmmo = _ammoSpentTimer = _elapsedTime = 0;
	}

	//__________________________________________________________________

	void CIronHellGoat::onAvailable() {
		CShoot::onAvailable();
		_currentSpentAmmo = _ammoSpentTimer = _elapsedTime = 0;
	}

	//__________________________________________________________________

	void CIronHellGoat::onTick(unsigned int msecs) {
		CShoot::onTick(msecs);

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
				(*it)->alterDirection( Math::getDirection( _entity->getOrientation() ) );
			}
		}
		else {
			// Si no nos quedan bolas vivas ponemos el componente a dormir
			// @todo Hay que cambiar el accept para que no acepte mensajes de control en general
			//putToSleep();
		}
	}

	//__________________________________________________________________

	void CIronHellGoat::primaryShoot() {
		// Si tenemos suficiente munición
		if(_primaryCanShoot && _currentAmmo > 0) {
			_primaryFireIsActive = true;
			decrementAmmo();
			++_currentSpentAmmo;

			// @deprecated Temporal hasta que este bien implementado
			CHudWeapons* hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
			if(hudWeapon != NULL)
				hudWeapon->loadingWeapon(true);
		}
		else if(_currentAmmo == 0) {
			// Si no tenemos suficiente munición ponemos el sonido de sin balas
		}
	}

	//__________________________________________________________________

	void CIronHellGoat::stopPrimaryShoot() {
		if(!_primaryFireIsActive) return;
		
		_primaryFireIsActive = false;

		// Obtenemos la información estandard asociada a la bola de fuego
		Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("FireBall")->clone();

		// Calculamos los valores customizados para la creacion de la bola de fuego
		// en funcion del tiempo que hemos mantenido pulsado el disparo primario
		float fireBallRadius = _defaultFireBallRadius + (_fireBallRadiusTemporalIncrement * _elapsedTime);
		float fireBallSpeed = _defaultFireBallSpeed + (_fireBallSpeedTemporalIncrement * _elapsedTime);
		float fireBallExplotionRadius = _defaultFireBallExplotionRadius + (_fireBallExplotionRadiusTemporalIncrement * _elapsedTime);
		float fireBallDamage = _defaultFireBallDamage + (_fireBallDamageTemporalIncrement * _elapsedTime);

		// Modificamos sus parámetros en base a los valores calculados
		entityInfo->setAttribute( "physic_radius", toString(fireBallRadius) );
		entityInfo->setAttribute( "scale", toString( Vector3(1.0f, 1.0f, 1.0f) * (fireBallRadius / _defaultFireBallRadius) ) );
		entityInfo->setAttribute( "speed", toString(fireBallSpeed) );
		entityInfo->setAttribute( "explotionRadius", toString(fireBallExplotionRadius) );
		entityInfo->setAttribute( "damage", toString(fireBallDamage) );
		entityInfo->setAttribute( "direction", toString( Math::getDirection( _entity->getTransform() ) ));

		// El único atributo que nos interesa pasar al cliente es la escala para que la bola
		// aparezca gráficamente más grande o más pequeña
		Map::CEntity* clientEntityInfo = new Map::CEntity( entityInfo->getName() );
		clientEntityInfo->setAttribute( "scale", toString( Vector3(1.0f, 1.0f, 1.0f) * (fireBallRadius / _defaultFireBallRadius) ) );

		// Creamos la bola de fuego con los parámetros customizados a partir
		// del radio de la cápsula + el radio de la bola + un pequeño offset
		Matrix4 shootTransform = _entity->getTransform();
		Vector3 shootPosition = shootTransform.getTrans() + ( Math::getDirection( _entity->getOrientation() ) * (_capsuleRadius + fireBallRadius + 0.5f) );
		shootPosition.y += _heightShoot - fireBallRadius;
		shootTransform.setTrans(shootPosition);

		// Creamos la entidad
		CEntity* fireBall = CEntityFactory::getSingletonPtr()->createCustomClientEntity(
								entityInfo,
								clientEntityInfo,
								CServer::getSingletonPtr()->getMap(),
								shootTransform
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

		// Emitimos el sonido de lanzar la bola de fuego
		emitSound(_shootAudio, "fireBallShot");

		// Me apunto la entidad devuelta por la factoria
		_controllableFireBalls.insert(fbController);
		
		// Reseteamos el reloj
		_currentSpentAmmo = _ammoSpentTimer = _elapsedTime = 0;

		// Seteamos el timer del cooldown a 0, para que empiece la cuenta aqui
		_primaryCooldownTimer = 0;
		_primaryCanShoot = false;

		// @deprecated Temporal hasta que este bien implementado
		CHudWeapons* hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
		if(hudWeapon != NULL) {
			hudWeapon->loadingWeapon(false);
			hudWeapon->shootAnim(-1.35f);
		}
	}

	//__________________________________________________________________

	void CIronHellGoat::secondaryShoot() {
		_secondaryFireIsActive = true;
	}

	//__________________________________________________________________

	void CIronHellGoat::stopSecondaryShoot() {
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
		CShootProjectile::resetAmmo();
		// Limpiamos la lista de bolas controlables
		if( !_controllableFireBalls.empty() ) {
			for(auto it = _controllableFireBalls.begin(); it != _controllableFireBalls.end(); ++it) {
				(*it)->setOwner(NULL);
			}
			_controllableFireBalls.clear();
		}
	}

}//namespace Logic