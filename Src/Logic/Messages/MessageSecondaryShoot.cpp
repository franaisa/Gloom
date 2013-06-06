#include "MessageSecondaryShoot.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSecondaryShoot);

	CMessageSecondaryShoot::CMessageSecondaryShoot() : CMessage(Message::SECONDARY_SHOOT) {
		// Nada que hacer
	}

	CMessageSecondaryShoot::CMessageSecondaryShoot(bool shoot) : _shoot(shoot), CMessage(Message::SECONDARY_SHOOT) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageSecondaryShoot::serialize() {
		Net::CBuffer buffer( sizeof(int) );
		buffer.serialize( std::string("CMessageSecondaryShoot"), true );
		buffer.serialize(_shoot);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSecondaryShoot::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_shoot);
	}

};
