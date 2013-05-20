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
									 _secondaryFireIsActive(false) {
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

		return true;
	}

	//__________________________________________________________________

	void CIronHellGoat::onActivate() {
		_fireBallRadius = _fireBallSpeed = _fireBallExplotionRadius = _fireBallDamage = 0.0f;
	}

	//__________________________________________________________________

	void CIronHellGoat::onAvailable() {
		_fireBallRadius = _fireBallSpeed = _fireBallExplotionRadius = _fireBallDamage = 0.0f;
	}

	//__________________________________________________________________

	void CIronHellGoat::onTick(unsigned int msecs) {
		// CShoot::onTick(msecs);

		// Si el jugador esta dejando pulsado el disparo primario, aumentamos
		// el tamaño de la bola y reducimos la velocidad hasta un limite
		if(_primaryFireIsActive) {
			// Si la bola no tiene el tamaño máximo, aumentarla conforme
			// al factor de crecimiento establecido y reducir su velocidad
			_fireBallRadius += 0.001f * msecs;
			_fireBallSpeed = 0.001f * msecs;
			_fireBallExplotionRadius += _fireBallRadius * 5.0f;
			_fireBallDamage += 0.001f * msecs;
		}
		else if(_secondaryFireIsActive) {
			// Si hay bolas vivas, mover las en función de la velocidad que
			// llevan y la dirección en la que apunto con el ratón

			// Comprobar el buffer de bolas vivas
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

		// Modificamos sus parámetros en base a los valores calculados
		entityInfo->setAttribute( "physic_radius", toString(_fireBallRadius) );
		entityInfo->setAttribute( "scale", toString( Vector3(1.0f, 1.0f, 1.0f) * _fireBallRadius ) );
		entityInfo->setAttribute( "speed", toString(_fireBallSpeed) );
		entityInfo->setAttribute( "explotionRadius", toString(_fireBallExplotionRadius) );
		entityInfo->setAttribute( "damage", toString(_fireBallDamage) );
		entityInfo->setAttribute( "direction", toString( Math::getDirection( _entity->getTransform() ) ));

		// Creamos la bola de fuego con los parámetros customizados a partir
		// del radio de la cápsula + el radio de la bola + un pequeño offset
		Matrix4 playerTransform = _entity->getTransform();
		Vector3 shootPosition = playerTransform.getTrans() + ( Math::getDirection( _entity->getOrientation() ) * (_capsuleRadius + _fireBallRadius + 1.0f) );
		shootPosition.y += _heightShoot - _fireBallRadius;

		// Creamos la entidad
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
		
		// Reseteo los valores de creación
		_fireBallRadius = _fireBallSpeed = _fireBallExplotionRadius = _fireBallDamage = 0.0f;
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