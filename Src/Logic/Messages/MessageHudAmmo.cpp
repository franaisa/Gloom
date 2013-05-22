#include "MessageHudAmmo.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudAmmo);

	CMessageHudAmmo::CMessageHudAmmo() : CMessage(Message::HUD_AMMO) {
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
		
	Net::CBuffer CMessageHudAmmo::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_ammo) + sizeof(_weapon));
		buffer.serialize(std::string("CMessageHudAmmo"),true);
		buffer.serialize(_ammo);
		buffer.serialize(_weapon);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudAmmo::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_ammo);
		buffer.deserialize(_weapon);
	}

};
