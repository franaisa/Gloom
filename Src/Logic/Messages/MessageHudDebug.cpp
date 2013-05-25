#include "MessageHudDebug.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudDebug);

	CMessageHudDebug::CMessageHudDebug() : CMessage(Message::HUD_DEBUG) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageHudDebug::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize(std::string("CMessageHudDebug"),true);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudDebug::deserialize(Net::CBuffer& buffer) {
	}

};
