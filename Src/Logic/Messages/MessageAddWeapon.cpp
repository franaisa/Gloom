#include "MessageAddWeapon.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddWeapon);

	CMessageAddWeapon::CMessageAddWeapon() : CMessage(Message::ADD_WEAPON) {
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
		
	Net::CBuffer CMessageAddWeapon::serialize() {
		
		Net::CBuffer buffer(sizeof(int) + sizeof(_weapon) + sizeof(_ammo));
		buffer.serialize(std::string("CMessageAddWeapon"),true);
		buffer.serialize(_weapon);
		buffer.serialize(_ammo);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAddWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
		buffer.deserialize(_ammo);
	}

};
