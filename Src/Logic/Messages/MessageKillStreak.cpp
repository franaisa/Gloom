#include "MessageKillStreak.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageKillStreak);

	
	CMessageKillStreak::CMessageKillStreak() : CMessage(Message::KILL_STREAK) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageKillStreak::serialize() {
		Net::CBuffer buffer(sizeof(int)*2);
		buffer.serialize(std::string("CMessageKillStreak"),true);
		buffer.serialize(_killer);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageKillStreak::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_killer);
	}

};
