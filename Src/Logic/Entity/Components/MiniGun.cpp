/**
@file MiniGun.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CMiniGun
@see Logic::IWeapon
*/

#include "MiniGun.h"
#include "HudWeapons.h"
#include "ScreamerShieldDamageNotifier.h"

// Mapa
#include "BaseSubsystems/Euler.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageControl.h"
#include "Audio/Server.h"

#include "Physics/Server.h"

#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageCreateParticle.h"

namespace Logic {
	IMP_FACTORY(CMiniGun);
	
	CMiniGun::CMiniGun() : IWeapon("miniGun"), 
							_primaryFireCooldown(0),
							_defaultPrimaryFireCooldown(0),
							_primaryFireCooldownTimer(0),
							_primaryFireIsActive(false),
							_secondaryFireCooldown(0),
							_defaultSecondaryFireCooldown(0),
							_secondaryFireCooldownTimer(0),
							_secondaryFireIsActive(false),
							_ammoSpentTimer(0),
							_ammoSpentTimeStep(0),
							_defaultAmmoSpentTimeStep(0),
							_currentSpentSecondaryAmmo(0),
							_maxAmmoSpentPerSecondaryShot(0),
							_secondaryFireLoadTime(0){
			
			// Nada que inicializar
		}

	//__________________________________________________________________

	CMiniGun::~CMiniGun() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CMiniGun::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		Map::CEntity* weapon = CEntityFactory::getSingletonPtr()->getInfo(_weaponName);

		if( !IWeapon::spawn(entity, map, weapon) ) return false;

		// Nos aseguramos de tener los atributos obligatorios
		assert( weapon->hasAttribute( "PrimaryFireCooldown") );
		assert( weapon->hasAttribute( "PrimaryFireDamage") );
		assert( weapon->hasAttribute( "ShotsDistance") );
		assert( weapon->hasAttribute( "PrimaryFireDispersion") );
		assert( weapon->hasAttribute( "PrimaryFireDispersionReductionPerShoot") );
		assert( weapon->hasAttribute( "SecondaryFireCooldown") );
		assert( weapon->hasAttribute( "SecondaryFireLoadTime") );
		assert( weapon->hasAttribute( "MaxAmmoSpentPerSecondaryShot") );
		

		_defaultPrimaryFireCooldown = _primaryFireCooldown = weapon->getFloatAttribute( "PrimaryFireCooldown") * 1000;
		_defaultDamage = _damage = weapon->getIntAttribute( "PrimaryFireDamage");


		// Distancia máxima de disparo
		_distance = weapon->getFloatAttribute( "ShotsDistance");

		// Atributos opcionales de audio
		/*if( entityInfo->hasAttribute( "Audio") )
			_audioShoot = entityInfo->getStringAttribute( "Audio");*/

		//Dispersión
		_dispersionOriginal = _dispersion = weapon->getFloatAttribute("PrimaryFireDispersion");
		_dispersionReductionPerShoot =  weapon->getFloatAttribute("PrimaryFireDispersionReductionPerShoot");

		// Cooldown del disparo secundario
		_defaultSecondaryFireCooldown = _secondaryFireCooldown = weapon->getFloatAttribute( "SecondaryFireCooldown") * 1000;

		// Tiempo de carga del arma
		_secondaryFireLoadTime = weapon->getFloatAttribute( "SecondaryFireLoadTime") * 1000;

		// Ratio al que gastamos municion
		_maxAmmoSpentPerSecondaryShot = weapon->getIntAttribute( "MaxAmmoSpentPerSecondaryShot");
		_defaultAmmoSpentTimeStep = _ammoSpentTimeStep = (float)_secondaryFireLoadTime / (float)(_maxAmmoSpentPerSecondaryShot);


		return true;
	}
	//__________________________________________________________________

	void CMiniGun::onActivate() {
		_currentSpentSecondaryAmmo = _ammoSpentTimer = 0;

		/*
		//Reiniciamos el cooldown
		reduceCooldown(0);
		//y los daños
		amplifyDamage(0);
		*/
	}
	//__________________________________________________________________

	void CMiniGun::onFixedTick(unsigned int msecs) 
	{
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0){
				_primaryFireCooldownTimer = 0;
				// siempre primary fire, ya que si en cualquier momento tengo q dejar de disparar, ya me llegara el mensaje
				if(_primaryFireIsActive)
					primaryFire();
			}
		}

		if(_secondaryFireIsActive) {
			if(_currentAmmo > 0 && _currentSpentSecondaryAmmo < _maxAmmoSpentPerSecondaryShot) {
				// Actualizamos el timer que se encarga de reducir la municion
				_ammoSpentTimer += msecs;
				if(_ammoSpentTimer >= _ammoSpentTimeStep) {
					++_currentSpentSecondaryAmmo;
					_ammoSpentTimer = 0;
				}
			}
		}
		
		// Controlamos el cooldown
		if(_secondaryFireCooldownTimer > 0) {
			_secondaryFireCooldownTimer -= msecs;
			
			if(_secondaryFireCooldownTimer < 0)
				_secondaryFireCooldownTimer = 0;
		}

	}

	//__________________________________________________________________

	void CMiniGun::primaryFire()
	{
		_primaryFireCooldownTimer = _primaryFireCooldown;

		_primaryFireIsActive = true;

		
		if(_dispersion){ // Si mayor que cero
			_dispersion = std::max(0.0f, _dispersion - _dispersionReductionPerShoot);
			//_dispersion = _dispersion < 0 ? 0 : _dispersion;
		}

		shoot();
		
	} // primaryFire
	//__________________________________________________________________
	
	void CMiniGun::stopPrimaryFire() 
	{
		_primaryFireIsActive = false;

		_dispersion = _dispersionOriginal;
		//paro el sonido de la mingun
		//emitSound("shotgun.wav", false, true, false);

	} // stopPrimaryFire
	//__________________________________________________________________
	
	void CMiniGun::secondaryFire()
	{
		_secondaryFireCooldownTimer = _secondaryFireCooldown;

		_secondaryFireIsActive = true;
	} // secondaryFire
	//__________________________________________________________________

	void CMiniGun::stopSecondaryFire() 
	{
		_secondaryFireIsActive = false;
		
		secondaryShoot();
	} // stopSecondaryFire
	//__________________________________________________________________

	void CMiniGun::amplifyDamage(unsigned int percentage) 
	{
		_damage = (percentage == 0) ? _defaultDamage : (_defaultDamage + (percentage * _damage * 0.01f));
	}
	//_________________________________________________

	void CMiniGun::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown - (percentage * _primaryFireCooldown * 0.01f));

		_secondaryFireCooldown = percentage == 0 ? _defaultSecondaryFireCooldown : (_defaultSecondaryFireCooldown - (percentage * _secondaryFireCooldown * 0.01f));

		_ammoSpentTimeStep = percentage == 0 ? _defaultAmmoSpentTimeStep : (_defaultAmmoSpentTimeStep - (percentage * _ammoSpentTimeStep * 0.01f));
		
	}

	void CMiniGun::shoot()
	{
		CEntity* entityHit = fireWeapon();
		if(entityHit != NULL) 
		{
			triggerHitMessages(entityHit);
		}

	}
	//__________________________________________________________________

	CEntity* CMiniGun::fireWeapon() 
	{
		

		//Direccion
		Vector3 direction = _entity->getOrientation() * Vector3::NEGATIVE_UNIT_Z; 
		//Me dispongo a calcular la desviacion del arma, en el map.txt se pondra en grados de dispersion (0 => sin dispersion)
		Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))/100.0f) * (_dispersion)) /100);
		//Esto hace un random total, lo que significa, por ejemplo, que puede que todas las balas vayan hacia la derecha 
		Vector3 dispersionDirection = direction.randomDeviant(angle);
		dispersionDirection.normalise();

		//El origen debe ser mínimo la capsula (si chocamos el disparo en la capsula al mirar en diferentes direcciones ya esta tratado en la funcion de colision)
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, dispersionDirection);
			
		// Dibujamos el rayo en ogre para poder depurar
		//drawRaycast(ray);

		decrementAmmo();

		//Comprobación de si da al mundo
		Physics::CRaycastHit hits2;
		bool disp = Physics::CServer::getSingletonPtr()->raycastSingle(ray, _distance,hits2, Physics::CollisionGroup::eWORLD);
		if (disp)
		{
			//Mandar el mensaje de los decal
			Vector3 pos = hits2.impact;
			drawDecal(hits2.entity, hits2.impact);
			
			// Añado aqui las particulas de dado en la pared.
			/*auto m = std::make_shared<CMessageCreateParticle>();
			m->setPosition(hits2.impact);
			m->setParticle("impactParticle");
			m->setDirectionWithForce(hits2.normal);
			hits2.entity->emitMessage(m);*/

			Euler euler(Quaternion::IDENTITY);
			euler.setDirection(hits2.normal);
			euler.yaw( Ogre::Radian(Math::HALF_PI) );

			Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("BulletSpark");
			CEntity* bulletSpark = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, _entity->getMap(), hits2.impact, euler.toQuaternion() );
			bulletSpark->activate();
			bulletSpark->start();

			int randomValue = Math::unifRand(2);
			std::string ricochetSound = (randomValue == 1 ? "weapons/hit/ric3.wav" : "weapons/hit/ric2.wav");
			Audio::CServer::getSingletonPtr()->playSound3D(ricochetSound, hits2.impact, Vector3::ZERO, false, false);
		}

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _distance,hits, true, Physics::CollisionGroup::ePLAYER | Physics::CollisionGroup::eSCREAMER_SHIELD | Physics::CollisionGroup::eHITBOX);

		//Devolvemos lo primero tocado que no seamos nosotros mismos
		CEntity* touched=NULL;
		for(int i=0;i<hits.size();++i) {
			if(hits[i].entity!=_entity) {
				if(hits[i].entity->getType() == "ScreamerShield") 
				{
					CEntity* screamerShieldOwner = hits[i].entity->getComponent<CScreamerShieldDamageNotifier>("CScreamerShieldDamageNotifier")->getOwner();

					// Si se trata de nuestro propio escudo salir
					if(screamerShieldOwner == _entity)
						break;
				}
				else 
				{
					touched=hits[i].entity;

					Euler euler(Quaternion::IDENTITY);
					euler.setDirection(hits[i].normal);

					Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("BloodStrike");
					CEntity* bloodStrike = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, _entity->getMap(), hits[i].impact, euler.toQuaternion() );
					bloodStrike->activate();
					bloodStrike->start();

					break;
				}
			}
		}

		// Creamos particulas de sangre
		if(touched != NULL) {
			
		}

		return touched;
		
	}// fireWeapon

	//__________________________________________________________________

	void CMiniGun::triggerHitMessages(CEntity* entityHit) 
	{
		std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
		m->setDamage(_damage);
		m->setEnemy(_entity);
		entityHit->emitMessage(m);
	}// triggerHitMessages

	//__________________________________________________________________


	void CMiniGun::secondaryShoot() 
	{
		if( !isActivated() ) return;

		//Creación de sweephit para 
		Physics::SphereGeometry sphere  = Physics::CGeometryFactory::getSingletonPtr()->createSphere(3.5);
		std::vector<Physics::CSweepHit> hits;
		//Physics::CServer::getSingletonPtr()->sweepMultiple(sphere, (_entity->getPosition() + Vector3(0,_heightShoot,0)),_directionShoot,_screamerScreamMaxDistance,hitSpots, true);
		Vector3 vDirectionShoot =_entity->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
		vDirectionShoot.normalise();
		Physics::CServer::getSingletonPtr()->sweepMultiple(sphere, (_entity->getPosition() + Vector3(0,_heightShoot,0)),vDirectionShoot, _distance,hits, false, Physics::CollisionGroup::ePLAYER | Physics::CollisionGroup::eSCREAMER_SHIELD );	

		for(auto it = hits.begin(); it < hits.end(); ++it){
			std::string typeEntity = (*it).entity->getType();
			if((*it).entity != _entity)
			{
				if( (*it).entity->getType() == "ScreamerShield") 
				{
					CEntity* screamerShieldOwner = (*it).entity->getComponent<CScreamerShieldDamageNotifier>("CScreamerShieldDamageNotifier")->getOwner();

					if(screamerShieldOwner == _entity)
						break;
				}

				int danyoTotal = _damage * _currentSpentSecondaryAmmo;
				std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
				m->setDamage(danyoTotal);
				m->setEnemy(_entity);
				(*it).entity->emitMessage(m);

				Euler euler(Quaternion::IDENTITY);
				euler.setDirection((*it).normal);

				Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("BloodStrike");
				CEntity* bloodStrike = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, _entity->getMap(), (*it).impact, euler.toQuaternion() );
				bloodStrike->activate();
				bloodStrike->start();
			}
		}

		//Decal, lo calculo sin dispersión
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton
		Vector3 direction= _entity->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
		direction.normalise();
		Ray ray(origin, direction);
		Physics::CRaycastHit hits2;
		bool disp = Physics::CServer::getSingletonPtr()->raycastSingle(ray, _distance,hits2, Physics::CollisionGroup::eWORLD);
		if (disp)
		{
			//Mandar el mensaje de los decal
			Vector3 pos = hits2.impact;
			drawDecal(hits2.entity, hits2.impact);

			Euler euler(Quaternion::IDENTITY);
			euler.setDirection(hits2.normal);
			euler.yaw( Ogre::Radian(Math::HALF_PI) );

			Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("BulletSpark");
			CEntity* bulletSpark = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, _entity->getMap(), hits2.impact, euler.toQuaternion() );
			bulletSpark->activate();
			bulletSpark->start();

			int randomValue = Math::unifRand(2);
			std::string ricochetSound = (randomValue == 1 ? "weapons/hit/ric3.wav" : "weapons/hit/ric2.wav");
			Audio::CServer::getSingletonPtr()->playSound3D(ricochetSound, hits2.impact, Vector3::ZERO, false, false);
		}

	} // secondaryShoot

} // namespace Logic

