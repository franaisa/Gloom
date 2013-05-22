#include "MessageSide.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSide);

	CMessageSide::CMessageSide() : CMessage(Message::SIDE) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageSide::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize(std::string("CMessageSide"), true);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSide::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
