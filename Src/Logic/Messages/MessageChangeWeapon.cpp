#include "MessageChangeWeapon.h"

namespace Logic {

	CMessageChangeWeapon::CMessageChangeWeapon(TMessageType type = TMessageType::CHANGE_WEAPON) : CMessage(type) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
	void CMessageChangeWeapon::setWeapon(int weapon){
		_weapon = weapon;
	}//
	//----------------------------------------------------------
	int CMessageChangeWeapon::getWeapon(){
		return _weapon;
	}//
	//----------------------------------------------------------
	Net::CBuffer CMessageChangeWeapon::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_weapon, sizeof(_weapon));
		_tempBuffer.reset();

		return _tempBuffer;
	}

};
