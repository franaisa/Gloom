#include "MessageAddWeapon.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddWeapon);

	CMessageAddWeapon::CMessageAddWeapon() : CMessage(TMessageType::ADD_WEAPON) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAddWeapon::setAddWeapon( int weapon){
		_weapon = weapon;
	}//
	//----------------------------------------------------------
	
	void CMessageAddWeapon::setAddAmmo( int ammo){
		_ammo = ammo;
	}//

	int CMessageAddWeapon::getAddWeapon(){
		return _weapon;
	}//
	//----------------------------------------------------------

	int CMessageAddWeapon::getAddAmmo(){
		return _ammo;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageAddWeapon::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_weapon) + sizeof(_ammo));
		_tempBuffer->serialize(std::string("CMessageAddWeapon"));
		_tempBuffer->serialize(_weapon);
		_tempBuffer->serialize(_ammo);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAddWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
		buffer.deserialize(_ammo);
	}

};
