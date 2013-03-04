#include "MessageAvatarWalk.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAvatarWalk);
	
	
	CMessageAvatarWalk::CMessageAvatarWalk() : CMessage(Message::AVATAR_WALK) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAvatarWalk::setDirection(Vector3 direction){
		_direction = direction;
	}//
	//----------------------------------------------------------

	Vector3 CMessageAvatarWalk::getDirection(){
		return _direction;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageAvatarWalk::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_direction.x) * 3);
		_tempBuffer->serialize(std::string("CMessageAvatarWalk"),true);
		_tempBuffer->serialize(_direction);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAvatarWalk::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_direction);
	}
};
