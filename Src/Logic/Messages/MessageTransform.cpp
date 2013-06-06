#include "MessageTransform.h"

#include "Logic/Entity/MessageFactory.h"
#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageTransform);

	CMessageTransform::CMessageTransform() : CMessage(Message::SET_TRANSFORM) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Vector3 CMessageTransform::getPosition(){
		return _position;
	}//
	//----------------------------------------------------------

	void CMessageTransform::setPosition(Vector3 position){
		_position=position;
	}//
	//----------------------------------------------------------

	Quaternion CMessageTransform::getOrientation(){
		return _orientation;
	}//
	//----------------------------------------------------------

	void CMessageTransform::setOrientation(Quaternion orientation){
		_orientation=orientation;
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

	Net::CBuffer CMessageTransform::serialize() {
		Net::CBuffer buffer(sizeof(int)+( sizeof(float) * 6)+sizeof(bool));
		buffer.serialize(std::string("CMessageTransform"),true);
		buffer.serialize(_position);
		buffer.serialize(_orientation);
		buffer.serialize(_convertCoordsToLogicWorld);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageTransform::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_position);
		buffer.deserialize(_orientation);
		buffer.deserialize(_convertCoordsToLogicWorld);
	}

};
