#include "MessageActivateScreamerShield.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageActivateScreamerShield);

	CMessageActivateScreamerShield::CMessageActivateScreamerShield() : 
		CMessage(Message::ACTIVATE_SCREAMER_SHIELD) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	CMessageActivateScreamerShield::~CMessageActivateScreamerShield() {
		// Nada que borrar
	}
	//----------------------------------------------------------

	Net::CBuffer CMessageActivateScreamerShield::serialize() {
		Net::CBuffer buffer( sizeof(int) );
		buffer.serialize( std::string("CMessageActivateScreamerShield"), true );
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageActivateScreamerShield::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
