#include "MessageSetPhysicPosition.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetPhysicPosition);
	
	
	CMessageSetPhysicPosition::CMessageSetPhysicPosition() : CMessage(TMessageType::SET_PHYSIC_POSITION) {
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

	Net::CBuffer* CMessageSetPhysicPosition::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_position.x) * 3);
		_tempBuffer->serialize(std::string("CMessageSetPhysicPosition"), true);
		_tempBuffer->serialize(_position);
		_tempBuffer->serialize(_convertCoordsToLogicWorld);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSetPhysicPosition::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_position);
		buffer.deserialize(_convertCoordsToLogicWorld);
	}
};
