#include "MessageKinematicMove.h"

namespace Logic {

	CMessageKinematicMove::CMessageKinematicMove(TMessageType type) : CMessage(type) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageKinematicMove::setMovement(Vector3 movement){
		_movement = movement;
	}//
	//----------------------------------------------------------

	Vector3 CMessageKinematicMove::getMovement(){
		return _movement;
	}//
	//----------------------------------------------------------
	Net::CBuffer CMessageKinematicMove::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_movement, sizeof(_movement));
		_tempBuffer.reset();

		return _tempBuffer;
	}

};
