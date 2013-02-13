#include "MessageSleep.h"

#include <string>

namespace Logic {

	CMessageSleep::CMessageSleep() : CMessage(TMessageType::SLEEP) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageSleep::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageSleep"));
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSleep::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
