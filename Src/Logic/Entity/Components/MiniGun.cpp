/**
@file MiniGun.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CMiniGun
@see Logic::IWeapon
*/

#include "MiniGun.h"
#include "HudWeapons.h"

// Mapa
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageControl.h"


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
		if( !IWeapon::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener los atributos obligatorios
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireCooldown") );
		assert( entityInfo->hasAttribute(_weaponName + "PrimaryFireDamage") );
		assert( entityInfo->hasAttribute(_weaponName + "Dispersion") );
		assert( entityInfo->hasAttribute(_weaponName + "Distance") );

		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireCooldown") * 1000;
		_defaultDamage = _damage = entityInfo->getIntAttribute(_weaponName + "PrimaryFireDamage");

		// Dispersión del arma
		//_dispersionOriginal = _dispersion = entityInfo->getFloatAttribute(_weaponName + "Dispersion");

		// Distancia máxima de disparo
		_distance = entityInfo->getFloatAttribute(_weaponName + "ShotsDistance");

		// Atributos opcionales de audio
		/*if( entityInfo->hasAttribute(_weaponName + "Audio") )
			_audioShoot = entityInfo->getStringAttribute(_weaponName + "Audio");*/

		//Dispersión
		_dispersion = entityInfo->getFloatAttribute(_weaponName+"PrimaryFireDispersion");
		_dispersionOriginal = _dispersion;

		// Cooldown del disparo secundario
		_defaultSecondaryFireCooldown = _secondaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "SecondaryFireCooldown") * 1000;

		// Tiempo de carga del arma
		_secondaryFireLoadTime = entityInfo->getFloatAttribute(_weaponName + "SecondaryFireLoadTime") * 1000;

		// Ratio al que gastamos municion
		_maxAmmoSpentPerSecondaryShot = entityInfo->getIntAttribute(_weaponName + "MaxAmmoSpentPerSecondaryShot");
		_defaultAmmoSpentTimeStep = _ammoSpentTimeStep = (float)_secondaryFireLoadTime / (float)(_maxAmmoSpentPerSecondaryShot);


		return true;
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

		shoot();
	} // primaryFire
	//__________________________________________________________________
	
	void CMiniGun::stopPrimaryFire() 
	{
		_primaryFireIsActive = false;
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
			//std::cout << "dado" << std::endl;
			triggerHitMessages(entityHit);
		}

	}
	//__________________________________________________________________

	CEntity* CMiniGun::fireWeapon() 
	{
		//Sonido
		emitSound(_weaponSound,_weaponName,false);

		//Direccion
		Vector3 direction = Math::getDirection(_entity->getOrientation()); 
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
			auto m = std::make_shared<CMessageCreateParticle>();
			m->setPosition(hits2.impact);
			m->setParticle("impactParticle");
			m->setDirectionWithForce(hits2.normal);
			hits2.entity->emitMessage(m);
		}

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _distance,hits, true,Physics::CollisionGroup::ePLAYER);

		//Devolvemos lo primero tocado que no seamos nosotros mismos
		CEntity* touched=NULL;
		for(int i=0;i<hits.size();++i)
			if(hits[i].entity!=_entity)
				touched=hits[i].entity;
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
		//Sonido
		emitSound(_weaponSound,_weaponName,false);

		//Creación de sweephit para 
		Physics::SphereGeometry sphere  = Physics::CGeometryFactory::getSingletonPtr()->createSphere(3.5);
		std::vector<Physics::CSweepHit> hits;
		//Physics::CServer::getSingletonPtr()->sweepMultiple(sphere, (_entity->getPosition() + Vector3(0,_heightShoot,0)),_directionShoot,_screamerScreamMaxDistance,hitSpots, true);
		Vector3 vDirectionShoot = Math::getDirection(_entity->getOrientation());
		Physics::CServer::getSingletonPtr()->sweepMultiple(sphere, (_entity->getPosition() + Vector3(0,_heightShoot,0)),vDirectionShoot, _distance,hits, false, Physics::CollisionGroup::ePLAYER );	

		for(auto it = hits.begin(); it < hits.end(); ++it){
			std::string typeEntity = (*it).entity->getType();
			if((*it).entity->getName() != _entity->getName())
			{
				int danyoTotal = _damage * _currentSpentSecondaryAmmo;
				std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
				m->setDamage(danyoTotal);
				m->setEnemy(_entity);
				(*it).entity->emitMessage(m);
			}
		}

		//Decal, lo calculo sin dispersión
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, Math::getDirection(_entity->getOrientation()));
		Physics::CRaycastHit hits2;
		bool disp = Physics::CServer::getSingletonPtr()->raycastSingle(ray, _distance,hits2, Physics::CollisionGroup::eWORLD);
		if (disp)
		{
			//Mandar el mensaje de los decal
			Vector3 pos = hits2.impact;
			drawDecal(hits2.entity, hits2.impact);
		}

	} // secondaryShoot

} // namespace Logic

