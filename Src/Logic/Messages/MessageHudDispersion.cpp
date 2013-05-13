#include "MessageHudDispersion.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudDispersion);

	CMessageHudDispersion::CMessageHudDispersion() : CMessage(Message::HUD_DISPERSION) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageHudDispersion::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageHudDispersion"), true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudDispersion::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
