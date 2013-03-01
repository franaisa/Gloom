#include "MessagePlayerDead.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerDead);

	
	CMessagePlayerDead::CMessagePlayerDead() : CMessage(Message::PLAYER_DEAD) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessagePlayerDead::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessagePlayerDead"),true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessagePlayerDead::deserialize(Net::CBuffer& buffer) {
	// Nada que hacer
	}

};
