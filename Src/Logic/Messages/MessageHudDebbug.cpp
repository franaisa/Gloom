#include "MessageHudDebbug.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudDebbug);

	CMessageHudDebbug::CMessageHudDebbug() : CMessage(Message::HUD_DEBBUG) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageHudDebbug::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageHudDebbug"),true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudDebbug::deserialize(Net::CBuffer& buffer) {
	}

};
