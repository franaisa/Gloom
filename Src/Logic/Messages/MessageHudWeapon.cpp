#include "MessageHudWeapon.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudWeapon);

	CMessageHudWeapon::CMessageHudWeapon() : CMessage(TMessageType::HUD_WEAPON) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageHudWeapon::setWeapon( int weapon){
		_weapon = weapon;
	}//
	//----------------------------------------------------------

	int CMessageHudWeapon::getWeapon(){
		return _weapon;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageHudWeapon::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_weapon));
		_tempBuffer->serialize("CMessageHudWeapon");
		_tempBuffer->serialize(_weapon);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
	}

};
