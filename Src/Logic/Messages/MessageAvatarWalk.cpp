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

	Net::CBuffer CMessageAvatarWalk::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_direction, sizeof(_direction));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
