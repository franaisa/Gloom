#include "MessageActivate.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageActivate);

	CMessageActivate::CMessageActivate() : CMessage(Message::ACTIVATE) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageActivate::serialize() {
		Net::CBuffer buffer( sizeof(int) );
		buffer.serialize( std::string("CMessageActivate"), true );
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageActivate::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
