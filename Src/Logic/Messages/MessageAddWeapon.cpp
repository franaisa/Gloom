#include "MessageAddWeapon.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddWeapon);

	CMessageAddWeapon::CMessageAddWeapon() : CMessage(TMessageType::ADD_LIFE) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAddWeapon::setAddWeapon( int weapon){
		_weapon = weapon;
	}//
	//----------------------------------------------------------

	int CMessageAddWeapon::getAddWeapon(){
		return _weapon;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageAddWeapon::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_weapon));
		_tempBuffer->serialize("CMessageAddWeapon");
		_tempBuffer->serialize(_weapon);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAddWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
	}

};
