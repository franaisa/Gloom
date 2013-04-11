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

	Net::CBuffer* CMessageActivateScreamerShield::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize( std::string("CMessageActivateScreamerShield"), true );
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageActivateScreamerShield::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
