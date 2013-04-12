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

	Net::CBuffer* CMessageDeactivateScreamerShield::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize( std::string("CMessageDeactivateScreamerShield"), true );
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageDeactivateScreamerShield::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
