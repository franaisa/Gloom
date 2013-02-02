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
	Net::CBuffer* CMessageKinematicMove::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(_movement.x) * 3);
		_tempBuffer->serialize(_movement);
		
		return _tempBuffer;
	}

};
