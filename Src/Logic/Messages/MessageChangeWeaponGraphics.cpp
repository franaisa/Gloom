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
	//----------------------------------------------------------
	Net::CBuffer* CMessageChangeWeaponGraphics::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(_weapon));
		_tempBuffer->serialize(_weapon);
		
		return _tempBuffer;
	}

};
