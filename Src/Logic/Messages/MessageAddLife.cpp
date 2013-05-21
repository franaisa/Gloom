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
		
	Net::CBuffer CMessageAddLife::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_life));
		buffer.serialize(std::string("CMessageAddLife"),true);
		buffer.serialize(_life);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAddLife::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_life);
	}

};
