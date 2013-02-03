#include "MessagePlayerDead.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerDead);

	
	CMessagePlayerDead::CMessagePlayerDead() : CMessage(TMessageType::PLAYER_DEAD) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	bool CMessagePlayerDead::getPlayerDead(){
		return _playerDead;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessagePlayerDead::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_playerDead));
		_tempBuffer->serialize("CMessagePlayerDead");
		_tempBuffer->serialize(_playerDead);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessagePlayerDead::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_playerDead);
	}

};
