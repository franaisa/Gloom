#include "MessageActivate.h"

#include <string>

namespace Logic {

	CMessageActivate::CMessageActivate() : CMessage(TMessageType::ACTIVATE) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageActivate::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageActivate"));
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageActivate::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
