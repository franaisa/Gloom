#include "MessageTransform.h"

#include "Logic/Entity/MessageFactory.h"
#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageTransform);

	CMessageTransform::CMessageTransform() : CMessage(Message::SET_TRANSFORM) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Matrix4 CMessageTransform::getTransform(){
		return _transform;
	}//
	//----------------------------------------------------------

	void CMessageTransform::setTransform(Matrix4 transform){
		_transform=transform;
	}//
	//----------------------------------------------------------
	
	void CMessageTransform::setMakeConversion(bool convertCoordsToLogicWorld) {
		_convertCoordsToLogicWorld = convertCoordsToLogicWorld;
	}//
	//----------------------------------------------------------

	bool CMessageTransform::getMakeConversion() {
		return _convertCoordsToLogicWorld;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageTransform::serialize() {
		assert(_tempBuffer == NULL);
		_tempBuffer = new Net::CBuffer(sizeof(int)+( sizeof(float) * 5)+sizeof(bool));
		_tempBuffer->serialize(std::string("CMessageTransform"),true);
		_tempBuffer->serialize(_transform);
		_tempBuffer->serialize(_convertCoordsToLogicWorld);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageTransform::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_transform);
		buffer.deserialize(_convertCoordsToLogicWorld);
	}

};
