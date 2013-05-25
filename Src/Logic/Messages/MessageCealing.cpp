#include "MessageCealing.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageCealing);

	CMessageCealing::CMessageCealing() : CMessage(Message::CEALING) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageCealing::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize(std::string("CMessageCealing"), true);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageCealing::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
