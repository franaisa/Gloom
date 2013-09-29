/**
@file ShotGun.cpp

Contiene la implementación del componente que representa a la escopeta.
 
@see Logic::ShotGun
@see Logic::CShootProjectile

@author Antonio Jesus Narváez 
@date mayo, 2013
*/

#include "ShotGun.h"


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
#include "Logic/Messages/MessageDecal.h"

namespace Logic {
	IMP_FACTORY(CShotGun);

	CShotGun::CShotGun() : IWeapon("shotGun"), 
		                             _dispersionAngle(0),
									 _primaryFireCooldownTimer(0) {

	}
	//________________________________________________

	CShotGun::~CShotGun() {
		// Nada que hacer
	}// CShotGun
	//________________________________________________

	bool CShotGun::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		Map::CEntity* weapon = CEntityFactory::getSingletonPtr()->getInfo(_weaponName);

		if( !IWeapon::spawn(entity, map, weapon) ) return false;

		// Nos aseguramos de tener todos los parametros que necesitamos
		assert( weapon->hasAttribute("ShootForce") );
		assert( weapon->hasAttribute("ProjectileRadius") );
		assert( weapon->hasAttribute("PrimaryFireDispersion") );
		assert( weapon->hasAttribute("DamageBurned") );
		assert( weapon->hasAttribute("PrimaryFireCooldown") );
		assert( weapon->hasAttribute("NumberOfShots") );

		assert( weapon->hasAttribute("PrimaryFireDamage") );

		// Leemos los atributos
		_projectileShootForce = weapon->getFloatAttribute("ShootForce");
		_projectileRadius = weapon->getFloatAttribute("ProjectileRadius");
		_dispersionAngle  = weapon->getFloatAttribute("PrimaryFireDispersion");
		_damageBurned = weapon->getFloatAttribute("DamageBurned");
		_defaultPrimaryFireCooldown = _primaryFireCooldown = weapon->getFloatAttribute("PrimaryFireCooldown") * 1000;
		_numberOfShots = weapon->getIntAttribute("NumberOfShots");
		_defaultPrimaryFireDamage = _primaryFireDamage = weapon->getFloatAttribute("PrimaryFireDamage");

		return true;
	}// spawn
	//________________________________________________

	void CShotGun::onActivate() {
		/*
		//Reiniciamos el cooldown
		reduceCooldown(0);
		//y los daños
		amplifyDamage(0);
		*/
	}

	//__________________________________________________________________

	void CShotGun::secondaryFire() {
		// yo le digo que deben de volver, y a partir de ahi, ellas solas encuentran el camino :D
		if(!_projectiles.empty()){
			for(auto it = _projectiles.begin(); it != _projectiles.end(); ++it){
				(*it)->getComponent<CMagneticBullet>("CMagneticBullet")->setProjectileDirection(_entity->getPosition() + Vector3(0,_heightShoot, 0));
			} 
		}
	} // secondaryShoot
	//__________________________________________________________________

	void CShotGun::onTick(unsigned int msecs) {
		//printf("\n Balas en la lista %d",_projectiles.size());
		/*
		// Controlamos el cooldown del disparo primario y secundario
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0)
				_primaryFireCooldownTimer = 0;
		}
		*/
	}
	//__________________________________________________________________

	void CShotGun::primaryFire() {
		/*std::shared_ptr<CMessageCreateParticle> particleMsg = std::make_shared<CMessageCreateParticle>();
		particleMsg->setParticle("test");
		Vector3 position2 = this->getEntity()->getPosition();
		position2.y += _heightShoot;

		float fOffset = 2.0f;
		std::cout << "posicion= " << position2 ;
		Vector3 orientation = Math::getDirection(this->getEntity()->getOrientation())*fOffset;			
		position2 += orientation;

		particleMsg->setPosition(position2);
		_entity->emitMessage(particleMsg);*/

		int shots = _numberOfShots <= _currentAmmo ? _numberOfShots : _currentAmmo;
		for(int i = 0; i < shots; ++i) {
			Vector3 direction = _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
			Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))*0.01f) * (_dispersionAngle)) *0.01f);
			Vector3 dispersionDirection = direction.randomDeviant(angle);
			dispersionDirection.normalise();

			Vector3 position = _entity->getPosition();
			position.y += _heightShoot;

			CEntity *projectileEntity= CEntityFactory::getSingletonPtr()->createEntity( 
				CEntityFactory::getSingletonPtr()->getInfo("MagneticBullet"),
				Logic::CServer::getSingletonPtr()->getMap(),
				position,
				Quaternion::IDENTITY
			);
			projectileEntity->activate();
			projectileEntity->start();
			projectileEntity->getComponent<CMagneticBullet>("CMagneticBullet")->setProperties(this, _projectileShootForce, dispersionDirection, _heightShoot, _primaryFireDamage, _damageBurned);
			_projectiles.insert(projectileEntity);
		}			
	} // fireWeapon
	//_________________________________________________


	void CShotGun::destroyProjectile(CEntity *projectile, CEntity *killedBy){
		if(killedBy->getType() == "PhysicWorld"){
			drawDecal(killedBy, projectile->getPosition(), (int)WeaponType::eSHOTGUN);

			// Añado aqui las particulas de dado en la pared.
			auto m = std::make_shared<CMessageCreateParticle>();
			m->setPosition(projectile->getPosition());
			m->setParticle("impactParticle");
			// esto no es correcto en realidad, pero hasata que los decals esten en el otro lao, lo dejo asi.
			m->setDirectionWithForce(-(_entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z));
			killedBy->emitMessage(m);
		}

		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(projectile,true);
		_projectiles.erase(projectile);
	} // destroyProjectile
	//_________________________________________________
	
	void CShotGun::amplifyDamage(unsigned int percentage) {

		if(percentage == 0) {
			_primaryFireDamage = _defaultPrimaryFireDamage;
		}
		// Sino aplicamos el porcentaje pasado por parámetro
		else {
			_primaryFireDamage += percentage * _primaryFireDamage * 0.01f;
		}
	}
	//_________________________________________________

	void CShotGun::setCurrentAmmo(unsigned int ammo){
		_currentAmmo = ammo;
	} // setCurrentAmmo
	//_________________________________________________

} // namespace Logic

