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

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

namespace Logic {
	
	IMP_FACTORY(CIronHellGoat);

	//__________________________________________________________________

	CIronHellGoat::CIronHellGoat() : CShootProjectile("ironHellGoat"),
									 _primaryFireIsActive(false),
									 _secondaryFireIsActive(false),
									 _elapsedTime(0) {
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
		/*assert( entityInfo->hasAttribute(weaponName + "MaximumLoadingTime") );
		assert( entityInfo->hasAttribute(weaponName + "DefaultFireBallRadius") );
		assert( entityInfo->hasAttribute(weaponName + "DefaultFireBallSpeed" ) );
		assert( entityInfo->hasAttribute(weaponName + "DefaultFireBallExplotionRadius") );
		assert( entityInfo->hasAttribute(weaponName + "DefaultFireBallDamage") );
		assert( entityInfo->hasAttribute(weaponName + "MaxFireBallRadius") );
		assert( entityInfo->hasAttribute(weaponName + "MaxFireBallSpeed") );
		assert( entityInfo->hasAttribute(weaponName + "MaxFireBallExplotionRadius") );
		assert( entityInfo->hasAttribute(weaponName + "MaxFireBallDamage") );

		// Tiempo de carga del arma
		_maxLoadingTime = entityInfo->getIntAttribute(weaponName + "MaximumLoadingTime") * 1000.0f;

		// Valores de creación de la bola de fuego por defecto
		_defaultFireBallRadius = entityInfo->getFloatAttribute(weaponName + "DefaultFireBallRadius");
		_defaultFireBallSpeed = entityInfo->getFloatAttribute(weaponName + "DefaultFireBallSpeed");
		_defaultFireBallExplotionRadius = entityInfo->getFloatAttribute(weaponName + "DefaultFireBallExplotionRadius");
		_defaultFireBallDamage = entityInfo->getFloatAttribute(weaponName + "DefaultFireBallDamage");

		// Valores de creación máximos de la bola de fuego
		float maxFireBallRadius = entityInfo->getFloatAttribute(weaponName + "MaxFireBallRadius");
		float maxFireBallSpeed = entityInfo->getFloatAttribute(weaponName + "MaxFireBallSpeed");
		float maxFireBallExplotionRadius = entityInfo->getFloatAttribute(weaponName + "MaxFireBallExplotionRadius");
		float maxFireBallDamage = entityInfo->getFloatAttribute(weaponName + "MaxFireBallDamage");*/

		// Tiempo máximo de carga de la bola
		_maxLoadingTime = 2.0f * 1000.0f;

		// Valores por defecto
		_defaultFireBallRadius = 2.0f;
		_defaultFireBallSpeed = 0.15f;
		_defaultFireBallExplotionRadius = 30.0f;
		_defaultFireBallDamage = 50.0f;

		// Valores máximos
		float maxFireBallRadius = 5.0f;
		float maxFireBallSpeed = 0.035f;
		float maxFireBallExplotionRadius = 30.0f;
		float maxFireBallDamage = 100.0f;

		// Calculamos los factores de crecimiento en funcion del tiempo
		// maximo de carga
		_fireBallRadiusGrowthFactor = (maxFireBallRadius - _defaultFireBallRadius) / _maxLoadingTime;
		_fireBallSpeedGrowthFactor = (maxFireBallSpeed - _defaultFireBallSpeed) / _maxLoadingTime;
		_fireBallExplotionRadiusGrowthFactor = (maxFireBallExplotionRadius - _defaultFireBallExplotionRadius) / _maxLoadingTime;
		_fireBallDamageGrowthFactor = (maxFireBallDamage - _defaultFireBallDamage) / _maxLoadingTime;

		return true;
	}

	//__________________________________________________________________

	void CIronHellGoat::onActivate() {
		_elapsedTime = 0;
	}

	//__________________________________________________________________

	void CIronHellGoat::onAvailable() {
		_elapsedTime = 0;
	}

	//__________________________________________________________________

	void CIronHellGoat::onTick(unsigned int msecs) {
		// CShoot::onTick(msecs);

		// Si el jugador esta dejando pulsado el disparo primario, aumentamos
		// el tamaño de la bola y reducimos la velocidad hasta un limite
		if(_primaryFireIsActive) {
			// Contamos el tiempo que hemos mantenido pulsado el raton
			if(_elapsedTime < _maxLoadingTime) {
				_elapsedTime += msecs;

				if(_elapsedTime >= _maxLoadingTime) {
					_elapsedTime = _maxLoadingTime;
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
			// Si no quedan bolas vivas poner a dormir
		}
	}

	//__________________________________________________________________

	void CIronHellGoat::primaryShoot() {
		std::cout << "Primario" << std::endl;
		// Si tenemos suficiente munición
			_primaryFireIsActive = true;
		// Si no tenemos suficiente munición ponemos el sonido de sin balas
	}

	//__________________________________________________________________

	void CIronHellGoat::stopPrimaryShoot() {
		_primaryFireIsActive = false;

		// Obtenemos la información estandard asociada a la bola de fuego
		Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("FireBall");
		
		// Calculamos los valores customizados para la creacion de la bola de fuego
		// en funcion del tiempo que hemos mantenido pulsado el disparo primario
		float fireBallRadius = _defaultFireBallRadius + (_fireBallRadiusGrowthFactor * _elapsedTime);
		float fireBallSpeed = _defaultFireBallSpeed + (_fireBallSpeedGrowthFactor * _elapsedTime);
		float fireBallExplotionRadius = _defaultFireBallExplotionRadius + (_fireBallExplotionRadiusGrowthFactor * _elapsedTime);
		float fireBallDamage = _defaultFireBallDamage + (_fireBallDamageGrowthFactor * _elapsedTime);

		// Modificamos sus parámetros en base a los valores calculados
		entityInfo->setAttribute( "physic_radius", toString(fireBallRadius) );
		//entityInfo->setAttribute( "scale", toString( Vector3(1.0f, 1.0f, 1.0f) * fireBallRadius ) );
		entityInfo->setAttribute( "speed", toString(fireBallSpeed) );
		entityInfo->setAttribute( "explotionRadius", toString(fireBallExplotionRadius) );
		entityInfo->setAttribute( "damage", toString(fireBallDamage) );
		entityInfo->setAttribute( "direction", toString( Math::getDirection( _entity->getTransform() ) ));

		// Creamos la bola de fuego con los parámetros customizados a partir
		// del radio de la cápsula + el radio de la bola + un pequeño offset
		Matrix4 playerTransform = _entity->getTransform();
		Vector3 shootPosition = playerTransform.getTrans() + ( Math::getDirection( _entity->getOrientation() ) * (_capsuleRadius + fireBallRadius + 0.5f) );
		shootPosition.y += _heightShoot - fireBallRadius;

		// Creamos la entidad
		/*CEntity* fireBall = CEntityFactory::getSingletonPtr()->createEntity( entityInfo, CServer::getSingletonPtr()->getMap() );*/
		CEntity* fireBall = CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation(
																	entityInfo, 
																	CServer::getSingletonPtr()->getMap(),
																	shootPosition,
																	Math::getYaw(playerTransform),
																	Math::getPitch(playerTransform)
																);

		// Le indicamos al controlador de la bola que este componente es el poseedor
		// para que se invoque al metodo correspondiente cuando las bolas mueran
		CFireBallController* fbController = fireBall->getComponent<CFireBallController>("CFireBallController");
		fbController->setOwner(this);
		
		// Arrancamos la entidad
		fireBall->activate();
		fireBall->start();

		// Me apunto la entidad devuelta por la factoria
		_controllableFireBalls.insert(fbController);
		
		// Reseteamos el reloj
		_elapsedTime = 0;
	}

	//__________________________________________________________________

	void CIronHellGoat::secondaryShoot() {
		std::cout << "Secundario" << std::endl;
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