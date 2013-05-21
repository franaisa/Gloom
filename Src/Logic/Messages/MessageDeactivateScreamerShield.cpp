#include "MessageDeactivateScreamerShield.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDeactivateScreamerShield);

	CMessageDeactivateScreamerShield::CMessageDeactivateScreamerShield() : 
		CMessage(Message::DEACTIVATE_SCREAMER_SHIELD) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	CMessageDeactivateScreamerShield::~CMessageDeactivateScreamerShield() {
		// Nada que borrar
	}

	Net::CBuffer CMessageDeactivateScreamerShield::serialize() {
		Net::CBuffer buffer(sizeof(int));
		buffer.serialize( std::string("CMessageDeactivateScreamerShield"), true );
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageDeactivateScreamerShield::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
