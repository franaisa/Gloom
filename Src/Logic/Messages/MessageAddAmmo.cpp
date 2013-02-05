#include "MessageAddAmmo.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddAmmo);

	CMessageAddAmmo::CMessageAddAmmo() : CMessage(TMessageType::ADD_LIFE) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAddAmmo::setAddAmmo( int ammo){
		_ammo = ammo;
	}//
	//----------------------------------------------------------

	int CMessageAddAmmo::getAddAmmo(){
		return _ammo;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageAddAmmo::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_ammo));
		_tempBuffer->serialize("CMessageAddAmmo");
		_tempBuffer->serialize(_ammo);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAddAmmo::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_ammo);
	}

};
