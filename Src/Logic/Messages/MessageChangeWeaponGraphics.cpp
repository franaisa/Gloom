#include "MessageChangeWeaponGraphics.h"

namespace Logic {

	CMessageChangeWeaponGraphics::CMessageChangeWeaponGraphics(TMessageType type) : CMessage(type) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
	void CMessageChangeWeaponGraphics::setWeapon(int weapon){
		_weapon = weapon;
	}//
	//----------------------------------------------------------
	int CMessageChangeWeaponGraphics::getWeapon(){
		return _weapon;
	}//
	Net::CBuffer CMessageChangeWeaponGraphics::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_weapon, sizeof(_weapon));
		_tempBuffer.reset();

		return _tempBuffer;
	}

};
