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
#include "Logic/Messages/MessageDecal.h"



namespace Logic {
	IMP_FACTORY(CShootShotGun);

	CShootShotGun::CShootShotGun() : IWeapon("shotGun"), 
		                             _dispersionAngle(0),
									 _primaryFireCooldownTimer(0) {

	}
	//________________________________________________

	CShootShotGun::~CShootShotGun() {
		// Nada que hacer
	}// CShootShotGun
	//________________________________________________

	bool CShootShotGun::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if(!IWeapon::spawn(entity, map, entityInfo))
			return false;

		// Nos aseguramos de tener todos los parametros que necesitamos
		assert( entityInfo->hasAttribute(_weaponName+"ShootForce") );
		assert( entityInfo->hasAttribute(_weaponName+"ProjectileRadius") );
		assert( entityInfo->hasAttribute(_weaponName+"PrimaryFireDispersion") );
		assert( entityInfo->hasAttribute(_weaponName+"DamageBurned") );
		assert( entityInfo->hasAttribute(_weaponName+"PrimaryFireCooldown") );
		assert( entityInfo->hasAttribute(_weaponName+"NumberOfShots") );

		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireDamage") );
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireDamage") );

		// Leemos los atributos
		_projectileShootForce = entityInfo->getFloatAttribute(_weaponName + "ShootForce");
		_projectileRadius = entityInfo->getFloatAttribute(_weaponName + "ProjectileRadius");
		_dispersionAngle  = entityInfo->getFloatAttribute(_weaponName+"PrimaryFireDispersion");
		_damageBurned = entityInfo->getFloatAttribute(_weaponName+"DamageBurned");
		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName+"PrimaryFireCooldown") * 1000;
		_numberOfShots = entityInfo->getIntAttribute(_weaponName+"NumberOfShots");
		_defaultPrimaryFireDamage = _primaryFireDamage = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireDamage");

		return true;
	}// spawn
	//________________________________________________

	void CShootShotGun::secondaryFire() {
		// yo le digo que deben de volver, y a partir de ahi, ellas solas encuentran el camino :D
		if(!_projectiles.empty()){
			for(auto it = _projectiles.begin(); it != _projectiles.end(); ++it){
				(*it)->getComponent<CMagneticBullet>("CMagneticBullet")->setProjectileDirection(_entity->getPosition() + Vector3(0,_heightShoot, 0));
			} 
		}
	} // secondaryShoot
	//__________________________________________________________________

	void CShootShotGun::onTick(unsigned int msecs) {
		// Controlamos el cooldown del disparo primario y secundario
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0)
				_primaryFireCooldownTimer = 0;
		}
	}
	//__________________________________________________________________

	void CShootShotGun::primaryFire() {
		_primaryFireCooldownTimer = _primaryFireCooldown;

		// Animacion de disparo
		// @deprecated Temporal hasta que este bien implementado
		CHudWeapons* hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
		if(hudWeapon != NULL)
			hudWeapon->shootAnim(-1.0f);

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

		decrementAmmo(shots);
	}
	//__________________________________________________________________

	void CShootShotGun::destroyProjectile(CEntity *projectile, CEntity *killedBy){
		if(killedBy->getType() == "World"){
			drawDecal(killedBy, projectile->getPosition());

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
	
	void CShootShotGun::amplifyDamage(unsigned int percentage) {
		if(percentage == 0) {
			_primaryFireDamage = _defaultPrimaryFireDamage;
		}
		// Sino aplicamos el porcentaje pasado por parámetro
		else {
			_primaryFireDamage += percentage * _primaryFireDamage * 0.01f;
		}
	}
	//_________________________________________________

	void CShootShotGun::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown  - (percentage * _primaryFireCooldown * 0.01f));
	}
	//_________________________________________________

	bool CShootShotGun::canUsePrimaryFire() {
		// Si tienes municion y el cooldown ha bajado
		return _primaryFireCooldownTimer == 0 && _currentAmmo > 0;
	}
	//_________________________________________________

	bool CShootShotGun::canUseSecondaryFire() {
		return true;
	}


} // namespace Logic

