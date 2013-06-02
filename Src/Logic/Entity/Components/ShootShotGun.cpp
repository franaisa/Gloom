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
#include "MagneticBullet.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "PhysicDynamicEntity.h"
#include "Map/MapEntity.h"
#include "HudWeapons.h"

//Debug
#include "Logic/Messages/MessageHudDebugData.h"
#include "Logic/Messages/MessageCreateParticle.h"


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

		if(entityInfo->hasAttribute(weapon+"DamageBurned")){
			_damageBurned = entityInfo->getFloatAttribute(weapon+"DamageBurned");
			
		}

		return true;
	}// spawn
	//________________________________________________
	
	void CShootShotGun::primaryShoot() {
		if(_primaryCanShoot && _currentAmmo > 0) {
			_primaryCanShoot = false;
			_primaryCooldownTimer = 0;
				
			drawParticle("shootParticle");

			float shoots = _numberShots;
			if(_currentAmmo < _numberShots)
				shoots = _currentAmmo;
			
			for(int i = 0; i < shoots ; ++i){
				fireWeapon();
				decrementAmmo();
			}

			//Sonido de disparo
			emitSound(_audioShoot, "audioShot");
			// @deprecated Temporal hasta que este bien implementado
			CHudWeapons* hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
			if(hudWeapon != NULL)
				hudWeapon->shootAnim(1.0f);
		}
		else if(_currentAmmo == 0) {
			// Ejecutar sonidos y animaciones de falta de balas
			emitSound(_noAmmo, "noAmmo", true);
		}
	} // primaryShoot
	//__________________________________________________________________

	void CShootShotGun::secondaryShoot() {
		// yo le digo que deben de volver, y a partir de ahi, ellas solas encuentran el camino :D
		if(!_projectiles.empty()){
			for(auto it = _projectiles.begin(); it != _projectiles.end(); ++it){
				(*it)->getComponent<CMagneticBullet>("CMagneticBullet")->setProjectileDirection(_entity->getPosition() + Vector3(0,_heightShoot, 0));
			} 
		}
	} // secondaryShoot
	//__________________________________________________________________

	void CShootShotGun::fireWeapon(){
		
		Vector3 direction = _entity->getQuatOrientation()*Vector3::NEGATIVE_UNIT_Z;
		Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))*0.01f) * (_dispersionAngle)) *0.01f);
		Vector3 dispersionDirection = direction.randomDeviant(angle);
		dispersionDirection.normalise();

		Vector3 position = _entity->getPosition();
		position.y += _heightShoot;

		//position += direction * (_capsuleRadius + _projectileRadius + 0.5 );
		Matrix4 transform = Matrix4::IDENTITY;
		transform.setTrans(position);

		CEntity *projectileEntity= CEntityFactory::getSingletonPtr()->createEntity( 
			CEntityFactory::getSingletonPtr()->getInfo("MagneticBullet"),
			Logic::CServer::getSingletonPtr()->getMap(),
			transform
		);
		projectileEntity->activate();
		projectileEntity->start();

		projectileEntity->getComponent<CMagneticBullet>("CMagneticBullet")->setProperties(this, _projectileShootForce, dispersionDirection, _heightShoot, _damage, _damageBurned);
		_projectiles.insert(projectileEntity);
			
	} // fireWeapon
	//_________________________________________________

	void CShootShotGun::destroyProjectile(CEntity *projectile, CEntity *killedBy){

		if(killedBy->getType() == "World"){
			decals(killedBy, projectile->getPosition());

			// Añado aqui las particulas de dado en la pared.
			auto m = std::make_shared<CMessageCreateParticle>();
			m->setPosition(projectile->getPosition());
			m->setParticle("impactParticle");
			// esto no es correcto en realidad, pero hasata que los decals esten en el otro lao, lo dejo asi.
			m->setDirectionWithForce(-(_entity->getQuatOrientation()*Vector3::NEGATIVE_UNIT_Z));
			killedBy->emitMessage(m);
		}

		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(projectile,true);
		_projectiles.erase(projectile);
	} // destroyProjectile
	//_________________________________________________
	
	/*
	void CShootShotGun::onFixedTick(unsigned int msecs){
	}

	*/
} // namespace Logic

