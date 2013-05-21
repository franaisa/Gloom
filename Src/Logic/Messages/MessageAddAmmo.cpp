#include "MessageAddAmmo.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddAmmo);

	CMessageAddAmmo::CMessageAddAmmo() : CMessage(Message::ADD_AMMO) {
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
		
	Net::CBuffer CMessageAddAmmo::serialize() {
		Net::CBuffer buffer( sizeof(int) + sizeof(_weapon) + sizeof(_ammo) );
		buffer.serialize(std::string("CMessageAddAmmo"),true);
		buffer.serialize(_weapon);
		buffer.serialize(_ammo);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAddAmmo::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_weapon);
		buffer.deserialize(_ammo);
	}

};
