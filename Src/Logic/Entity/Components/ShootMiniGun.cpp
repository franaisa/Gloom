/**
@file ShootMiniGun.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShootMiniGun
@see Logic::CShootRaycast
*/

#include "ShootMiniGun.h"

#include "Logic/Messages/MessageControl.h"

namespace Logic {
	IMP_FACTORY(CShootMiniGun);
	
	CShootMiniGun::~CShootMiniGun() {
		// Nada que hacer
	}

	void CShootMiniGun::process(const std::shared_ptr<CMessage>& message) {
		ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				
				if(type==Control::LEFT_CLICK) {
					_pressThenShoot=true;
				}
				else if(type==Control::UNLEFT_CLICK) {
					_pressThenShoot=false;
				}
				else if(type==Control::RIGHT_CLICK) {
					_acumulando = true;
				}
				else if(type==Control::UNRIGHT_CLICK) {
					std::cout << _contador << std::endl;
					_iRafagas = _contador / 10;
					_acumulando = false;
					_contador = 0;
				}
				
				break;
			}
		}
	} // process
	//__________________________________________________________________

	void CShootMiniGun::onTick(unsigned int msecs) {
		if(_primaryCooldownTimer < _primaryCooldown)
		{
			_primaryCooldownTimer += msecs;
		}
		else
		{
			if(_pressThenShoot){
				_primaryCanShoot=true;
				primaryShoot();
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
				std::cout << "disparo" << std::endl;
				_primaryCanShoot = true; //Ponemos este flag para 'trucar' el disparo y que se salte el cooldown
				primaryShoot();
				--_iRafagas; //disminuimos el número de ráfagas
			}
		}
	}	
	//__________________________________________________________________


} // namespace Logic

