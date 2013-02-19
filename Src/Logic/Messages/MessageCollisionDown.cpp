#include "MessageCollisionDown.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageCollisionDown);

	CMessageCollisionDown::CMessageCollisionDown(): CMessage(TMessageType::COLLISION_DOWN){
		
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

	Net::CBuffer* CMessageCollisionDown::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_collision));
		_tempBuffer->serialize(std::string("CMessageCollisionDown"),true);
		_tempBuffer->serialize(_collision);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageCollisionDown::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_collision);
	}
};