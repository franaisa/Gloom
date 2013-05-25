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
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudDispersion::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
