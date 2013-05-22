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

	Net::CBuffer CMessageFlash::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize(std::string("CMessageFlash"), true);
		buffer.serialize(_flashFactor);
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageFlash::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_flashFactor);
	}

};
