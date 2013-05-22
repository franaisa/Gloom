#include "MessageSetPhysicPosition.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetPhysicPosition);
	
	
	CMessageSetPhysicPosition::CMessageSetPhysicPosition() : CMessage(Message::SET_PHYSIC_POSITION) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageSetPhysicPosition::setPosition(const Vector3& direction) {
		_position = direction;
	}//
	//----------------------------------------------------------

	Vector3 CMessageSetPhysicPosition::getPosition(){
		return _position;
	}//
	//----------------------------------------------------------

	void CMessageSetPhysicPosition::setMakeConversion(bool convertCoordsToLogicWorld) {
		this->_convertCoordsToLogicWorld = convertCoordsToLogicWorld;
	}//
	//----------------------------------------------------------

	bool CMessageSetPhysicPosition::getMakeConversion() {
		return _convertCoordsToLogicWorld;
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessageSetPhysicPosition::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_position.x) * 3);
		buffer.serialize(std::string("CMessageSetPhysicPosition"), true);
		buffer.serialize(_position);
		buffer.serialize(_convertCoordsToLogicWorld);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSetPhysicPosition::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_position);
		buffer.deserialize(_convertCoordsToLogicWorld);
	}
};
