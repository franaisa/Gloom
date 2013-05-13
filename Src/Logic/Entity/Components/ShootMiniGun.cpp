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
				break;
			}
		}
	} // process
	//__________________________________________________________________

	void CShootMiniGun::onTick(unsigned int msecs) {
		if(_cooldownTimer < _cooldown)
			_cooldownTimer += msecs;
		else{
			if(_pressThenShoot){
				_canShoot=true;
				primaryShoot();
			}
		}
	} // tick
	//__________________________________________________________________


} // namespace Logic

