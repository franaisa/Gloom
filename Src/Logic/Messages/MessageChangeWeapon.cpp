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
	Net::CBuffer* CMessageChangeWeapon::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(_weapon));
		_tempBuffer->serialize(_weapon);
		
		return _tempBuffer;
	}

};
