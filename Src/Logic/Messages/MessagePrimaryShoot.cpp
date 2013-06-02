#include "MessagePrimaryShoot.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePrimaryShoot);

	CMessagePrimaryShoot::CMessagePrimaryShoot(bool shoot) : _shoot(shoot), CMessage(Message::PRIMARY_SHOOT) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessagePrimaryShoot::serialize() {
		Net::CBuffer buffer( sizeof(int) );
		buffer.serialize( std::string("CMessagePrimaryShoot"), true );
		buffer.serialize(_shoot);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessagePrimaryShoot::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_shoot);
	}

};
