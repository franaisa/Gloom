#include "MessageCealing.h"

#include <string>

namespace Logic {

	CMessageCealing::CMessageCealing() : CMessage(TMessageType::CEALING) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageCealing::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageCealing"));
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageCealing::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
