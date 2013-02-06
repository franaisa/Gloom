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

	void CMessageHudAmmo::setWeapon( int weapon){
		_weapon = weapon;
	}//
	//----------------------------------------------------------

	int CMessageHudAmmo::getWeapon(){
		return _weapon;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageHudAmmo::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_ammo) + sizeof(_weapon));
		_tempBuffer->serialize("CMessageHudAmmo");
		_tempBuffer->serialize(_ammo);
		_tempBuffer->serialize(_weapon);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudAmmo::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_ammo);
		buffer.deserialize(_weapon);
	}

};
