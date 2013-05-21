#include "MessageChangeWeapon.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageChangeWeapon);

	CMessageChangeWeapon::CMessageChangeWeapon() : CMessage(Message::CHANGE_WEAPON) {
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
		Net::CBuffer buffer(sizeof(int) + sizeof(_weapon));
		buffer.serialize(std::string("CMessageChangeWeapon"),true);
		buffer.serialize(_weapon);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageChangeWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
	}

};
