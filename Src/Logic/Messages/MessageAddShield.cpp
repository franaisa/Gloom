#include "MessageAddShield.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddShield);

	CMessageAddShield::CMessageAddShield() : CMessage(TMessageType::ADD_SHIELD) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAddShield::setAddShield( int shield){
		_shield = shield;
	}//
	//----------------------------------------------------------

	int CMessageAddShield::getAddShield(){
		return _shield;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageAddShield::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_shield));
		_tempBuffer->serialize("CMessageAddShield");
		_tempBuffer->serialize(_shield);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAddShield::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_shield);
	}

};
