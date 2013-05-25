#include "MessageKinematicMove.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageKinematicMove);

	CMessageKinematicMove::CMessageKinematicMove() : CMessage(Message::KINEMATIC_MOVE) {
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
		Net::CBuffer buffer(sizeof(int) + sizeof(_movement.x) * 3);
		buffer.serialize(std::string("CMessageKinematicMove"),true);
		buffer.serialize(_movement);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageKinematicMove::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_movement);
	}

};
