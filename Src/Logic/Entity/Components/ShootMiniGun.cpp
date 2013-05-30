/**
@file ShootMiniGun.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShootMiniGun
@see Logic::IWeapon
*/

#include "ShootMiniGun.h"
#include "HudWeapons.h"

// Mapa
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageHudDispersion.h"

#include "Physics/Server.h"

#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageCreateParticle.h"

namespace Logic {
	IMP_FACTORY(CShootMiniGun);
	
	CShootMiniGun::CShootMiniGun() : IWeapon("miniGun"), 
									 _pressThenShoot(false), 
									 _contador(0), 
									 _acumulando(false),
									 _iRafagas(0), 
									 _bLeftClicked(false), 
									 _iContadorLeftClicked(0),
									 _primaryFireCooldownTimer(0),
									 _iMaxRafagas(20),
									 _bMensajeDispMandado(false) {
			
			// Nada que inicializar
		}

	//__________________________________________________________________

	CShootMiniGun::~CShootMiniGun() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CShootMiniGun::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
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
		_distance = entityInfo->getFloatAttribute(_weaponName + "Distance");

		// Atributos opcionales de audio
		/*if( entityInfo->hasAttribute(_weaponName + "Audio") )
			_audioShoot = entityInfo->getStringAttribute(_weaponName + "Audio");*/

		//Dispersión
		_dispersion = entityInfo->getFloatAttribute(_weaponName+"Dispersion");
		_dispersionOriginal = _dispersion;

		return true;
	}

	//__________________________________________________________________
	/*
	void CShootMiniGun::process(const std::shared_ptr<CMessage>& message) 
	{
		/*
		ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				
				if(type==Control::LEFT_CLICK) {
					_pressThenShoot=true;
					_bLeftClicked = true;
				}
				else if(type==Control::UNLEFT_CLICK) {
					_pressThenShoot=false;
					_bLeftClicked = false;
					_iContadorLeftClicked = 0;
					
					//Envío el mensaje con valores para que resetee la mirilla
					auto m = std::make_shared<CMessageHudDispersion>();
					m->setTime(0);
					m->setReset(true);
					_entity->emitMessage(m);

					_bMensajeDispMandado = false;
					std::cout << "cambio mensajo dispmandado " << _bMensajeDispMandado << std::endl;
				}
				else if(type==Control::RIGHT_CLICK) {
					_acumulando = true;
				}
				else if(type==Control::UNRIGHT_CLICK) {
					_iRafagas = _contador / 10;
					_acumulando = false;
					_contador = 0;
				}
				
				break;
			}
		}*/
	//} // process

	//__________________________________________________________________

	void CShootMiniGun::onFixedTick(unsigned int msecs) 
	{
		// @deprecated Temporal hasta que este bien implementado
		CHudWeapons* hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
		if(hudWeapon != NULL)
			hudWeapon->continouosShooting(_bLeftClicked && _currentAmmo > 0);

		//std::cout << "fixed" << std::endl;
		if (_bLeftClicked) 
		{
			++_iContadorLeftClicked;
			
			//Modificar la dispersión
			if ((_iContadorLeftClicked < 10) && (!_bMensajeDispMandado)) 
			{
				_dispersion = _dispersionOriginal + 15.0f;
				//Enviamos el mensaje para que empiece a modificar la mirilla con la dispersión

				std::shared_ptr<CMessageHudDispersion> m = std::make_shared<CMessageHudDispersion>();
				m->setHeight(8.0f);
				m->setWidth(9.0f);
				m->setTime(2500);//Tiempo máximo que bajará el tamaño de la mirilla
				m->setReset(false);
				_entity->emitMessage(m);

				_bMensajeDispMandado = true;
				printf("\nReduciendo mira");
				/**
				NOTA: De momento tiene el bug de que si disparas cuando no tienes munición, sigue haciendo la dispersión.
				La movida es que se sabe si tienes munición o no en el método primaryShoot, de su padre ShootRaycast.
				Habría que hacer que este método fuese un booleano y devolviese true si ha podido disparar y false si no.
				De momento no lo cambio porque creo que se va a cambiar la forma de gestionar estos raycast más adelante,
				pero hay que tenerlo en cuenta (también se tiene que tener en cuenta para cuando se ponga la animación
				de vibración de la minigun).
				//*/
			}			
			else if (_iContadorLeftClicked < 20) {
				_dispersion = _dispersionOriginal + 5.0f;
			}
			else {
				_dispersion = _dispersionOriginal;
			}
		}

		// AQUI ESTA EL PROBLEMA, EL TIMER DEBERIA SER CONTROLADO
		// SOLO POR LA CLASE PADRE.
		// LO IDEAL ES QUE LAS CLASES HIJAS DEFINAN CUANDO SE PUEDE
		// USAR EL ARMA.
		// Podemos usar una implementacion por defecto pero imponer
		// metodos abstractos para el canUse, amplify y reduce. De
		// esta manera las nimiedades de cada arma quedan reservadas
		// a las armas en concreto aunque se impone una interfaz comun 
		if(_primaryFireCooldownTimer < _primaryFireCooldown) 
		{
			_primaryFireCooldownTimer += msecs;
		}
		else 
		{
			/*if(_pressThenShoot) {
				//_primaryCanShoot=true;				
				//primaryFire();
			}*/
			_primaryFireCooldownTimer = 0;
			if (_bLeftClicked && _currentAmmo > 0)
			{
				shoot();
			}
		}

		//Comprobamos la funcionalidad del botón derecho
		if (_acumulando) 
		{
			//Si tenemos el botón derecho pulsado, seguimos aumentando el contador
			_contador++;
		}
		else 
		{
			//No tenemos pulsado el derecho, así que comprobamos si tenemos rafagas que lanzar
			if (_iRafagas > 0) 
			{
				//Controlo que no se tengan más ráfagas del máximo (en su caso lo seteo a este valor)
				if (_iRafagas > _iMaxRafagas) 
				{
					_iRafagas = _iMaxRafagas;
				}

				secondaryShoot(_iRafagas);
				_iRafagas = 0;
			}
		}
	}

	//__________________________________________________________________

	bool CShootMiniGun::canUsePrimaryFire() 
	{
		return _currentAmmo > 0;
	}
	
	//__________________________________________________________________

	bool CShootMiniGun::canUseSecondaryFire() 
	{
		return !_bLeftClicked;
	}

	//__________________________________________________________________

	void CShootMiniGun::primaryFire()
	{
		_pressThenShoot=true;
		_bLeftClicked = true;
	}

	//__________________________________________________________________
	
	void CShootMiniGun::secondaryFire()
	{
		_acumulando = true;
	}

	//__________________________________________________________________


	void CShootMiniGun::stopPrimaryFire() 
	{
		_pressThenShoot=false;
		_bLeftClicked = false;
		_iContadorLeftClicked = 0;
					
		//Envío el mensaje con valores para que resetee la mirilla
		auto m = std::make_shared<CMessageHudDispersion>();
		m->setTime(0);
		m->setReset(true);
		_entity->emitMessage(m);

		_bMensajeDispMandado = false;

		_bLeftClicked = false;
	}
	//__________________________________________________________________

	void CShootMiniGun::stopSecondaryFire() 
	{
		_iRafagas = _contador / 10;
		_acumulando = false;
		_contador = 0;
	}
	//__________________________________________________________________

	void CShootMiniGun::amplifyDamage(unsigned int percentage) 
	{
		_damage = (percentage == 0) ? _defaultDamage : (_defaultDamage + (percentage * _damage * 0.01f));
	}
	//_________________________________________________

	void CShootMiniGun::reduceCooldown(unsigned int percentage) 
	{
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		_primaryFireCooldown = percentage == 0 ? _defaultPrimaryFireCooldown : (_defaultPrimaryFireCooldown - (percentage * _primaryFireCooldown * 0.01f));
	}
	//__________________________________________________________________

	void CShootMiniGun::shoot()
	{
		CEntity* entityHit = fireWeapon();
		if(entityHit != NULL) 
		{
			std::cout << "dado" << std::endl;
			triggerHitMessages(entityHit);
		}
	}
	//__________________________________________________________________

	CEntity* CShootMiniGun::fireWeapon() 
	{
		//Direccion
		Vector3 direction = Math::getDirection(_entity->getOrientation()); 
		//Me dispongo a calcular la desviacion del arma, en el map.txt se pondra en grados de dispersion (0 => sin dispersion)
		Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))/100.0f) * (_dispersion)) /100);
		//Esto hace un random total, lo que significa, por ejemplo, que puede que todas las balas vayan hacia la derecha 
		Vector3 dispersionDirection = direction.randomDeviant(angle);
		dispersionDirection.normalise();

		std::cout << "Angulo: " << angle << std::endl;

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

	void CShootMiniGun::triggerHitMessages(CEntity* entityHit) 
	{
		std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
		m->setDamage(_damage);
		m->setEnemy(_entity);
		entityHit->emitMessage(m);
	}// triggerHitMessages

	//__________________________________________________________________


	void CShootMiniGun::secondaryShoot(int iRafagas) 
	{
		decrementAmmo(iRafagas);

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
				int danyoTotal = _damage * iRafagas;
				std::cout << "Le he dado!!! Danyo = " << danyoTotal << std::endl;

				std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
				m->setDamage(danyoTotal);
				m->setEnemy(_entity);
				(*it).entity->emitMessage(m);

				//Le he dado
				/*
				Vector3 direct = -(_directionShoot.reflect(-(*it).normal));
				auto m = std::make_shared<CMessageAddForcePlayer>();
				m->setForce(_directionShoot * (_screamerScreamForce*(1.0f- (*it).distance/_screamerScreamMaxDistance)));
				(*it).entity->emitMessage(m);

				auto m2 = std::make_shared<CMessageSetAnimation>();
				m2->setString("Damage");
				m2->setBool(false);
				(*it).entity->emitMessage(m2);*/
			}
		}
	} // secondaryShoot

} // namespace Logic

