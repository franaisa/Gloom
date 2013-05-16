#include "MessageFlash.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageFlash);

	CMessageFlash::CMessageFlash() : CMessage(Message::FLASH) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	void CMessageFlash::setFlashFactor(const float &flashFactor){_flashFactor = flashFactor;}

	//----------------------------------------------------------
	
	float CMessageFlash::getFlashFactor(){return _flashFactor;}

	//----------------------------------------------------------

	Net::CBuffer* CMessageFlash::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageFlash"), true);
		_tempBuffer->serialize(_flashFactor);
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageFlash::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_flashFactor);
	}

};
