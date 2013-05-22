#include "MessageDeactivate.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDeactivate);

	CMessageDeactivate::CMessageDeactivate() : CMessage(Message::DEACTIVATE) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageDeactivate::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize(std::string("CMessageDeactivate"), true);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageDeactivate::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
