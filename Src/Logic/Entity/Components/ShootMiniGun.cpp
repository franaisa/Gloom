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
	


	void CShootMiniGun::process(CMessage *message) {
		switch(message->getMessageType()) {
			case Message::CONTROL:
				if(((CMessageControl*)message)->getType()==Control::LEFT_CLICK) {
					_pressThenShoot=true;
				}
				else if(((CMessageControl*)message)->getType()==Control::UNLEFT_CLICK) {
					_pressThenShoot=false;
				}
				break;
			break;
		}
	} // process
	//__________________________________________________________________

	void CShootMiniGun::tick(unsigned int msecs) {
		IComponent::tick(msecs);
		//std::cout << "HOLA EJECUTO EL TICK DE LA MINIGUN" << std::endl;

		if(_cooldownTimer < _cooldown)
			_cooldownTimer += msecs;
		else{
			if(_pressThenShoot){
				_canShoot=true;
				shoot();
			}
		}
	} // tick
	//__________________________________________________________________

} // namespace Logic

