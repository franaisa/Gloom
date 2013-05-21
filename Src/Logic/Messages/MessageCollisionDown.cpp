#include "MessageCollisionDown.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageCollisionDown);

	CMessageCollisionDown::CMessageCollisionDown(): CMessage(Message::COLLISION_DOWN){
		
	}//
	//----------------------------------------------------------

	void CMessageCollisionDown::setCollisionDown(bool collision){
		_collision = collision;
	}//
	//----------------------------------------------------------

	bool CMessageCollisionDown::getCollisionDown(){
		return _collision;
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessageCollisionDown::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_collision));
		buffer.serialize(std::string("CMessageCollisionDown"),true);
		buffer.serialize(_collision);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageCollisionDown::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_collision);
	}
};