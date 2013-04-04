#include "MessageHudDebug.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudDebug);

	CMessageHudDebug::CMessageHudDebug() : CMessage(Message::HUD_DEBUG) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageHudDebug::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageHudDebug"),true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudDebug::deserialize(Net::CBuffer& buffer) {
	}

};
