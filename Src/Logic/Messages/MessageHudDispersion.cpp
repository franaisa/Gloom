#include "MessageHudDispersion.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudDispersion);

	CMessageHudDispersion::CMessageHudDispersion() : CMessage(Message::HUD_DISPERSION) {
		// Nada que hacer
		_fWidth = -1;
		_fHeight = -1;
		_iTime = -1;
		_bReset = false;
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageHudDispersion::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_fWidth) + sizeof(_fHeight) + sizeof(_iTime) + sizeof(_bReset));
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
