#include "MessageWakeUp.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageWakeUp);

	CMessageWakeUp::CMessageWakeUp() : CMessage(Message::WAKEUP) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageWakeUp::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageWakeUp"), true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageWakeUp::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
