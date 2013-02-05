#include "MessageHudLife.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudLife);

	CMessageHudLife::CMessageHudLife() : CMessage(TMessageType::HUD_LIFE) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageHudLife::setLife( int life){
		_life = life;
	}//
	//----------------------------------------------------------

	int CMessageHudLife::getLife(){
		return _life;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageHudLife::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_life));
		_tempBuffer->serialize("CMessageHudLife");
		_tempBuffer->serialize(_life);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudLife::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_life);
	}

};
