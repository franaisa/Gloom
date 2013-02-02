#include "MessageAvatarWalk.h"

namespace Logic {

	CMessageAvatarWalk::CMessageAvatarWalk(TMessageType type) : CMessage(type) {
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

		_tempBuffer = new Net::CBuffer(sizeof(_direction.x) * 3);
		_tempBuffer->serialize(_direction);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
