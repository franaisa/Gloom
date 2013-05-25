#include "MessageWakeUp.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageWakeUp);

	CMessageWakeUp::CMessageWakeUp() : CMessage(Message::WAKEUP) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageWakeUp::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize(std::string("CMessageWakeUp"), true);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageWakeUp::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
