#include "MessageDeactivate.h"

#include <string>

namespace Logic {

	CMessageDeactivate::CMessageDeactivate() : CMessage(TMessageType::DEACTIVATE) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageDeactivate::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageDeactivate"));
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageDeactivate::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
