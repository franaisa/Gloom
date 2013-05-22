#include "MessageHudWeapon.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudWeapon);

	CMessageHudWeapon::CMessageHudWeapon() : CMessage(Message::HUD_WEAPON) {
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

	void CMessageHudWeapon::setAmmo( int ammo){
		_ammo = ammo;
	}//
	//----------------------------------------------------------

	int CMessageHudWeapon::getAmmo(){
		return _ammo;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageHudWeapon::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_ammo) + sizeof(_weapon));
		buffer.serialize(std::string("CMessageHudWeapon"),true);
		buffer.serialize(_ammo);
		buffer.serialize(_weapon);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_ammo);
		buffer.deserialize(_weapon);
	}

};
