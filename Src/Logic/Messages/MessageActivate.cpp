#include "MessageActivate.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageActivate);

	CMessageActivate::CMessageActivate() : CMessage(TMessageType::ACTIVATE) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageActivate::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageActivate"), true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageActivate::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
