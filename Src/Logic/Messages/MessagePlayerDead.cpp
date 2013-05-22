#include "MessagePlayerDead.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerDead);

	
	CMessagePlayerDead::CMessagePlayerDead() : CMessage(Message::PLAYER_DEAD) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessagePlayerDead::serialize() {
		Net::CBuffer buffer(sizeof(int)*2);
		buffer.serialize(std::string("CMessagePlayerDead"),true);
		buffer.serialize(_killer);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessagePlayerDead::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_killer);
	}

};
