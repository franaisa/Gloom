#include "MessageWakeUp.h"

#include <string>

namespace Logic {

	CMessageWakeUp::CMessageWakeUp() : CMessage(TMessageType::WAKEUP) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageWakeUp::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageWakeUp"));
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageWakeUp::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
