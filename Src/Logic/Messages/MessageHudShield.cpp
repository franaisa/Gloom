#include "MessageHudShield.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudShield);

	CMessageHudShield::CMessageHudShield() : CMessage(Message::HUD_SHIELD) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageHudShield::setShield( int shield){
		_shield = shield;
	}//
	//----------------------------------------------------------

	int CMessageHudShield::getShield(){
		return _shield;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageHudShield::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_shield));
		buffer.serialize(std::string("CMessageHudShield"),true);
		buffer.serialize(_shield);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudShield::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_shield);
	}

};
