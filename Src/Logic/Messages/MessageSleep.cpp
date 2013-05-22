#include "MessageSleep.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSleep);

	CMessageSleep::CMessageSleep() : CMessage(Message::SLEEP) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageSleep::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize(std::string("CMessageSleep"), true);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSleep::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
