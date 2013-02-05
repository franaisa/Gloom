#include "MessageHudAmmo.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudAmmo);

	CMessageHudAmmo::CMessageHudAmmo() : CMessage(TMessageType::HUD_AMMO) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageHudAmmo::setAmmo( int ammo){
		_ammo = ammo;
	}//
	//----------------------------------------------------------

	int CMessageHudAmmo::getAmmo(){
		return _ammo;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageHudAmmo::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_ammo));
		_tempBuffer->serialize("CMessageHudAmmo");
		_tempBuffer->serialize(_ammo);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudAmmo::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_ammo);
	}

};
