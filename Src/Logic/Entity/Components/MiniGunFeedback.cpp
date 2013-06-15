/**
@file MiniGun.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CMiniGunFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#include "MiniGunFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageHudDispersion.h"

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

		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireCooldown") * 1000;

		_dispersionOriginal = _dispersion = entityInfo->getFloatAttribute(_weaponName+"PrimaryFireDispersion");
		
		return true;
	}

	//__________________________________________________________________

	void CMiniGunFeedback::primaryFire() {
		// Poner la animacion de carga inestable del arma
		_hudWeapon->continouosShooting(true);
	}

	//__________________________________________________________________

	void CMiniGunFeedback::stopPrimaryFire() {
		// Parar la animacion de carga
		_hudWeapon->continouosShooting(false);
		
		_iContadorLeftClicked = 0;
		//Env�o el mensaje con valores para que resetee la mirilla
		auto m = std::make_shared<CMessageHudDispersion>();
		m->setTime(0);
		m->setReset(true);
		_entity->emitMessage(m);
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
	}

	//__________________________________________________________________

	void CMiniGunFeedback::onFixedTick(unsigned int msecs){
		if(_primaryFireCooldownTimer > 0) {
			_primaryFireCooldownTimer -= msecs;
			
			if(_primaryFireCooldownTimer < 0){
				_primaryFireCooldownTimer = _primaryFireCooldown;

				if(_primaryFireIsActive){
					// las alternativas son, o enviar un mensaje por cada disparo, o q el componente de shoot tb tenga el cooldown

					++_iContadorLeftClicked;

					//Modificar la dispersi�n
					if ((_iContadorLeftClicked < 10) && (!_bMensajeDispMandado)) 
					{
						_dispersion = _dispersionOriginal + 15.0f;
						//Enviamos el mensaje para que empiece a modificar la mirilla con la dispersi�n

						std::shared_ptr<CMessageHudDispersion> m = std::make_shared<CMessageHudDispersion>();
						m->setHeight(8.0f);
						m->setWidth(9.0f);
						m->setTime(2500);//Tiempo m�ximo que bajar� el tama�o de la mirilla
						m->setReset(false);
						_entity->emitMessage(m);

						_bMensajeDispMandado = true;
						//printf("\nReduciendo mira");
						/**
						NOTA: De momento tiene el bug de que si disparas cuando no tienes munici�n, sigue haciendo la dispersi�n.
						La movida es que se sabe si tienes munici�n o no en el m�todo primaryShoot, de su padre ShootRaycast.
						Habr�a que hacer que este m�todo fuese un booleano y devolviese true si ha podido disparar y false si no.
						De momento no lo cambio porque creo que se va a cambiar la forma de gestionar estos raycast m�s adelante,
						pero hay que tenerlo en cuenta (tambi�n se tiene que tener en cuenta para cuando se ponga la animaci�n
						de vibraci�n de la minigun).
						//*/
					}			
					else if (_iContadorLeftClicked < 20) {
						_dispersion = _dispersionOriginal + 5.0f;
					}
					else {
						_dispersion = _dispersionOriginal;
					}

				} // fin _primaryFireIsActive

			}
		}
	}
}//namespace Logic