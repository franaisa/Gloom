#include "MessageSide.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSide);

	CMessageSide::CMessageSide() : CMessage(Message::SIDE) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageSide::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageSide"), true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSide::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
