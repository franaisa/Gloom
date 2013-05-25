#include "MessageHudDispersion.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudDispersion);

	CMessageHudDispersion::CMessageHudDispersion() : CMessage(Message::HUD_DISPERSION) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageHudDispersion::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize(std::string("CMessageHudDispersion"), true);
		buffer.serialize(_fWidth);
		buffer.serialize(_fHeight);
		buffer.serialize(_iTime);
		buffer.serialize(_bReset);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudDispersion::deserialize(Net::CBuffer& buffer) {	
		buffer.deserialize(_fWidth);
		buffer.deserialize(_fHeight);
		buffer.deserialize(_iTime);
		buffer.deserialize(_bReset);
	}

};
