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

//Debug
#include "Logic/Messages/MessageHudDebugData.h"

namespace Logic {
	IMP_FACTORY(CShootShotGun);

	CShootShotGun::~CShootShotGun() {
		// Nada que hacer

	}// CShootShotGun
	//________________________________________________

	bool CShootShotGun::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if(!CShoot::spawn(entity, map, entityInfo))
			return false;

		// Leer los parametros que toquen para los proyectiles
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;	////!!!! Aqui debes de poner el nombre del arma que leera en el map.txt
		std::string weapon = aux.str();

		if(entityInfo->hasAttribute(weapon+"ShootForce"))
			_projectileShootForce = entityInfo->getFloatAttribute(weapon + "ShootForce");

		if(entityInfo->hasAttribute(weapon+"ProjectileRadius"))
			_projectileRadius = entityInfo->getFloatAttribute(weapon + "ProjectileRadius");

		if(entityInfo->hasAttribute("audioNoAmmo"))
			_noAmmo = entityInfo->getStringAttribute("audioNoAmmo");

		if(entityInfo->hasAttribute(weapon+"Audio"))
			_audioShoot = entityInfo->getStringAttribute(weapon+"Audio");

		if(entityInfo->hasAttribute(weapon+"Dispersion")){
			_dispersionAngle  = entityInfo->getFloatAttribute(weapon+"Dispersion");
		}

		return true;
	}// spawn
	//________________________________________________
	
	void CShootShotGun::primaryShoot() {
		if(_primaryCanShoot && _currentAmmo > 0 && _numberShots <= _currentAmmo){
			_primaryCanShoot = false;
			_primaryCooldownTimer = 0;
				
			drawParticle("fire", "shootParticle");

			for(int i = 0; i < _numberShots; ++i){
				printf("\nDisparo");
				fireWeapon();
			}

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

			for(int i = 0; i < _numberShots; ++i){
				printf("\nDisparo secundario");
				//fireWeapon();
			}

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
		Vector3 dispersionDirection = direction.randomDeviant(Ogre::Radian(_dispersionAngle));
		dispersionDirection.normalise();
		Vector3 position = _entity->getPosition();
		position.y += _heightShoot;

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

	void CShootShotGun::onFixedTick(unsigned int msecs){
	
		for(auto it = _projectiles.begin(); it != _projectiles.end(); ++it){

			auto m = std::make_shared<CMessageHudDebugData>();
			m->setKey((*it)->getEntity()->getName());
			m->setValue((*it)->getEntity()->getPosition());
			_entity->emitMessage(m);

			_projectileDirection = Math::getDirection((*it)->getEntity()->getOrientation()) * _projectileShootForce;
			/*
			(*it)->move(_projectileDirection);
			/*/
			(*it)->getEntity()->setPosition((*it)->getEntity()->getPosition() + _projectileDirection);
			/* */
		}
	}

} // namespace Logic

