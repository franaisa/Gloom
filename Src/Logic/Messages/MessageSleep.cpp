#include "MessageSleep.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSleep);

	CMessageSleep::CMessageSleep() : CMessage(Message::SLEEP) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageSleep::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageSleep"), true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSleep::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
