/**
@file ShootShotGun.cpp

Contiene la implementación del componente que representa a la escopeta.
 
@see Logic::ShootShotGun
@see Logic::CShootProjectile

@author Antonio Jesus Narváez 
@date mayo, 2013
*/

#include "ShootShotGun.h"

// Logica
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "PhysicDynamicEntity.h"

namespace Logic {
	IMP_FACTORY(CShootShotGun);

	CShootShotGun::~CShootShotGun() {
		// Nada que hacer

	}// CShootShotGun
	//________________________________________________

	bool CShootShotGun::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if(!CShootProjectile::spawn(entity, map, entityInfo))
			return false;

		if(entityInfo->hasAttribute("shotGundispersion")){
			float _dispersionAngle  = entityInfo->getFloatAttribute("shotGundispersion");
		}

		return true;
	}// spawn
	//________________________________________________
	
	void CShootShotGun::primaryShoot() {
		if(_primaryCanShoot && _currentAmmo > 0 && _numberShots <= _currentAmmo){
			_primaryCanShoot = false;
			_primaryCooldownTimer = 0;
				
			drawParticle("fire", "shootParticle");

			for(int i = 0; i < _numberShots; ++i)
				printf("\nDisparo");
				//fireWeapon();

			decrementAmmo();

			//Sonido de disparo
			emitSound(_audioShoot, "audioShot");
		}
		else if(_currentAmmo == 0) {
			// Ejecutar sonidos y animaciones de falta de balas
			emitSound(_noAmmo, "noAmmo", true);
		}
	} // primaryShoot
	//__________________________________________________________________

	void CShootShotGun::secondaryShoot() {
		if(_primaryCanShoot && _currentAmmo > 0 && _numberShots <= _currentAmmo){
			_primaryCanShoot = false;
			_primaryCooldownTimer = 0;
				
			drawParticle("fire", "shootParticle");

			for(int i = 0; i < _numberShots; ++i)
				printf("\nDisparo secundario");
				//fireWeapon();

			decrementAmmo();

			//Sonido de disparo
			emitSound(_audioShoot, "audioShot");
		}
		else if(_currentAmmo == 0) {
			// Ejecutar sonidos y animaciones de falta de balas
			emitSound(_noAmmo, "noAmmo", true);
		}
	} // secondaryShoot
	//__________________________________________________________________

	void CShootShotGun::fireWeapon(){
		
		Vector3 direction = Math::getDirection(_entity->getOrientation());
		direction.y = _heightShoot;
		Vector3 dispersionDirection = direction.randomDeviant(Ogre::Radian(_dispersionAngle));
		dispersionDirection.normalise();
		Vector3 position = _entity->getPosition();
		position.y = _heightShoot;

		//*
		Quaternion aux = Quaternion(&dispersionDirection);
		
		_projectiles.push_back(CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation( 
			CEntityFactory::getSingletonPtr()->getInfo("ShotGunProjectile"),
			Logic::CServer::getSingletonPtr()->getMap(),
			position,
			aux.getYaw().valueRadians(),
			aux.getPitch().valueRadians()
			)->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity"));

		/*/
		float x = dispersionDirection.x;
		float y = dispersionDirection.y;
		float z = dispersionDirection.z;
		
		float yaw = -std::atan2(x, -z);
		float pitch = std::atan2(y, std::sqrt(x*x + z*z);
		
		_projectiles.push_back(CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation( 
			CEntityFactory::getSingletonPtr()->getInfo("ShotGunProjectile"),
			Logic::CServer::getSingletonPtr()->getMap(),
			position,
			yaw,
			pitch
			)->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity"));
		/* */	
			
			
	} // fireWeapon
	//_________________________________________________

	void CShootShotGun::onTick(unsigned int msecs){
		CShoot::onTick(msecs);
		
		/*
		for(int i = 0; i < _numberShots; ++i){
			_projectileDirection = Math::getDirection(_projectiles[i]->getEntity()->getOrientation()) * _projectileShootForce;
			_projectiles[i]->move(_projectileDirection);
		}
		*/
	}

} // namespace Logic

