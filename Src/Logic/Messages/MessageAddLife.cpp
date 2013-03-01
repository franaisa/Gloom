#include "MessageAddLife.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddLife);

	CMessageAddLife::CMessageAddLife() : CMessage(Message::ADD_LIFE) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAddLife::setAddLife( int life){
		_life = life;
	}//
	//----------------------------------------------------------

	int CMessageAddLife::getAddLife(){
		return _life;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageAddLife::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_life));
		_tempBuffer->serialize(std::string("CMessageAddLife"),true);
		_tempBuffer->serialize(_life);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAddLife::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_life);
	}

};
