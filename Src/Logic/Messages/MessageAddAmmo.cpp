#include "MessageAddAmmo.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddAmmo);

	CMessageAddAmmo::CMessageAddAmmo() : CMessage(TMessageType::ADD_AMMO) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAddAmmo::setAddWeapon( int weapon){
		_weapon = weapon;
	}//
	//----------------------------------------------------------
	
	void CMessageAddAmmo::setAddAmmo( int ammo){
		_ammo = ammo;
	}//

	int CMessageAddAmmo::getAddWeapon(){
		return _weapon;
	}//
	//----------------------------------------------------------

	int CMessageAddAmmo::getAddAmmo(){
		return _ammo;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageAddAmmo::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_weapon) + sizeof(_ammo));
		_tempBuffer->serialize(std::string("CMessageAddAmmo"));
		_tempBuffer->serialize(_weapon);
		_tempBuffer->serialize(_ammo);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAddAmmo::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
		buffer.deserialize(_ammo);
	}

};
