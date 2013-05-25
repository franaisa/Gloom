#include "MessageChangeGravity.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageChangeGravity);

	CMessageChangeGravity::CMessageChangeGravity() : CMessage(Message::CHANGE_GRAVITY) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageChangeGravity::setGravity(float gravity){
		_gravity = gravity;
	}//
	//----------------------------------------------------------

	float CMessageChangeGravity::getGravity(){
		return _gravity;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageChangeGravity::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_gravity));
		buffer.serialize(std::string("CMessageChangeGravity"),true);
		buffer.serialize(_gravity);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageChangeGravity::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_gravity);
	}

};
