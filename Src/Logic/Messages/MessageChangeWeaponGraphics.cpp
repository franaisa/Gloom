#include "MessageChangeWeaponGraphics.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageChangeWeaponGraphics);

	CMessageChangeWeaponGraphics::CMessageChangeWeaponGraphics() : CMessage(Message::CHANGE_WEAPON_GRAPHICS) {
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

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_weapon));
		_tempBuffer->serialize(std::string("CMessageChangeWeaponGraphics"),true);
		_tempBuffer->serialize(_weapon);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageChangeWeaponGraphics::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
	}

};
