#include "MessageKinematicMove.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageKinematicMove);

	CMessageKinematicMove::CMessageKinematicMove() : CMessage(TMessageType::KINEMATIC_MOVE) {
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

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_movement.x) * 3);
		_tempBuffer->serialize(std::string("CMessageKinematicMove"));
		_tempBuffer->serialize(_movement);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageKinematicMove::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_movement);
	}

};
