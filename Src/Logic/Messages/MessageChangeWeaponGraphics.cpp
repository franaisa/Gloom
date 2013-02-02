#include "MessageChangeWeaponGraphics.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageChangeWeaponGraphics);

	CMessageChangeWeaponGraphics::CMessageChangeWeaponGraphics() : CMessage(TMessageType::CHANGE_WEAPON_GRAPHICS) {
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
