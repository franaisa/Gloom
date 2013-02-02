#include "MessageChangeWeapon.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageChangeWeapon);

	CMessageChangeWeapon::CMessageChangeWeapon() : CMessage(TMessageType::CHANGE_WEAPON) {
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

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_weapon));
		_tempBuffer->serialize(std::string("CMessageChangeWeapon"));
		_tempBuffer->serialize(_weapon);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageChangeWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
	}

};
