/**
@file MiniGun.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CMiniGunFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "MiniGunFeedback.h"
#include "HudWeapons.h"
#include "DynamicLight.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageHudDispersion.h"
#include "Logic/Messages/MessageParticleStart.h"
#include "Logic/Messages/MessageParticleStop.h"
#include "Logic/Messages/MessageActivate.h"

#include "DynamicParticleSystem.h"


using namespace std;

namespace Logic {

	IMP_FACTORY(CMiniGunFeedback);

	//__________________________________________________________________

	CMiniGunFeedback::CMiniGunFeedback() : IWeaponFeedback("miniGun"),
										  _defaultPrimaryFireCooldown(0),
										  _primaryFireCooldown(0),
										  _primaryFireCooldownTimer(0),
									      _primaryFireIsActive(false),
										  _dispersion(0),
										  _dispersionOriginal(0),
										  _iContadorLeftClicked(0),
										  _bMensajeDispMandado(false){
		// Nada que hacer
	}

	//__________________________________________________________________

	CMiniGunFeedback::~CMiniGunFeedback() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CMiniGunFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponFeedback::spawn(entity, map, entityInfo) ) return false;

		Map::CEntity* weapon = CEntityFactory::getSingletonPtr()->getInfo(_weaponName);

		_defaultPrimaryFireCooldown = _primaryFireCooldown = weapon->getFloatAttribute("PrimaryFireCooldown") * 1000;

		_dispersionOriginal = _dispersion = weapon->getFloatAttribute("PrimaryFireDispersion");
		
		return true;
	}

	//__________________________________________________________________

	void CMiniGunFeedback::primaryFire() {
		// Poner la animacion de carga inestable del arma
		_hudWeapon->continouosShooting(true);
		//Sonido
		//emitSound(_weaponSound, true, true, false, false);
		
		// Shoot flash
		CDynamicLight* shootFlash = _entity->getComponent<CDynamicLight>("CDynamicLight");
		shootFlash->setColor( Vector3(0.098f, 0.823f, 0.161f) );
		shootFlash->setAttenuation( Vector3(1.0f, 0.014f, 0.0007f) );
		shootFlash->setRange(325.0f);
		shootFlash->turnOn( Vector3(0.0f, _heightShoot, 0.0f) );
		_firingRound = _primaryFireIsActive = true;

		emitParticle2();

		_currentParticle->getComponent<CDynamicParticleSystem>("CDynamicParticleSystem")->setVisible(true);
	}

	//__________________________________________________________________

	void CMiniGunFeedback::stopPrimaryFire() {
		// Parar la animacion de carga
		_hudWeapon->continouosShooting(false);
		//Sonido
		//emitSound("miniGun", true, true, false, true);
		
		_iContadorLeftClicked = 0;
		//Envío el mensaje con valores para que resetee la mirilla
		auto m = std::make_shared<CMessageHudDispersion>();
		m->setTime(0);
		m->setReset(true);
		_entity->emitMessage(m);

		_entity->getComponent<CDynamicLight>("CDynamicLight")->turnOff();
		_primaryFireIsActive = false;


		std::shared_ptr<CMessageActivate> activateMsg = std::make_shared<CMessageActivate>();
		activateMsg->setActivated(false);
		_currentParticle->emitMessage(activateMsg);
		printf("\n para de emitir");

		_currentParticle->getComponent<CDynamicParticleSystem>("CDynamicParticleSystem")->setVisible(false);
	}

	//__________________________________________________________________

	void CMiniGunFeedback::secondaryFire() {
		_hudWeapon->loadingWeapon(true);		
	}
	//__________________________________________________________________
	
	void CMiniGunFeedback::stopSecondaryFire() {
		// Parar la animacion de carga
		_hudWeapon->loadingWeapon(false);
		_hudWeapon->shootAnim(-1.85f);
		//Sonido
		//emitSound("miniGun", false, true, false, true);
		
		// Shoot flash
		CDynamicLight* shootFlash = _entity->getComponent<CDynamicLight>("CDynamicLight");
		shootFlash->setColor( Vector3(0.9f, 0.9f, 0.9f) );
		shootFlash->setAttenuation( Vector3(1.0f, 0.014f, 0.0007f) );
		shootFlash->setRange(325.0f);
		shootFlash->turnOn( Vector3(0.0f, _heightShoot, 0.0f), 0.1f );

		//emitParticle2(false);

		_currentParticle->getComponent<CDynamicParticleSystem>("CDynamicParticleSystem")->setVisible(false);
	}

	//__________________________________________________________________

	void CMiniGunFeedback::onFixedTick(unsigned int msecs){
		if(_primaryFireIsActive) {
			flashAnim();
		}
		
		/*if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;

			if(_primaryFireCooldownTimer < 0){
				_primaryFireCooldownTimer = _primaryFireCooldown;

				if(_primaryFireIsActive){
					// las alternativas son, o enviar un mensaje por cada disparo, o q el componente de shoot tb tenga el cooldown
					primaryFire();

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

						_bMensajeDispMandado = true;*/
						//printf("\nReduciendo mira");
						/**
						NOTA: De momento tiene el bug de que si disparas cuando no tienes munición, sigue haciendo la dispersión.
						La movida es que se sabe si tienes munición o no en el método primaryShoot, de su padre ShootRaycast.
						Habría que hacer que este método fuese un booleano y devolviese true si ha podido disparar y false si no.
						De momento no lo cambio porque creo que se va a cambiar la forma de gestionar estos raycast más adelante,
						pero hay que tenerlo en cuenta (también se tiene que tener en cuenta para cuando se ponga la animación
						de vibración de la minigun).
						//*/
					/*}			
					else if (_iContadorLeftClicked < 20) {
						_dispersion = _dispersionOriginal + 5.0f;
					}
					else {
						_dispersion = _dispersionOriginal;
					}

				} // fin _primaryFireIsActive

			}
		}*/
	}

	void CMiniGunFeedback::flashAnim() {
		// Shoot flash
		CDynamicLight* shootFlash = _entity->getComponent<CDynamicLight>("CDynamicLight");

		if(_firingRound) {
			float r = Math::unifRand(0.098f, 0.18f);
			float g = Math::unifRand(0.823f, 0.833f);
			float b = Math::unifRand(0.161f, 0.171f);
			
			shootFlash->setColor( Vector3(r, g, b) );
		}
		else {
			shootFlash->setColor( Vector3(0.0f, 0.0f, 0.0f) );
		}

		_firingRound = !_firingRound;
	}
}//namespace Logic