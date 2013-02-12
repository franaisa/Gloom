#include "MessageControl.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageControl);

	CMessageControl::CMessageControl( ) : CMessage(TMessageType::CONTROL) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	void CMessageControl::setType(ControlType controlType){
		_controlType=controlType;
	}//
	//----------------------------------------------------------

	ControlType CMessageControl::getType(){
		return _controlType;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageControl::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_controlType));
		_tempBuffer->serialize(std::string("CMessageControl"),true);
		_tempBuffer->serialize(_controlType);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageControl::deserialize(Net::CBuffer& buffer) {
		buffer.read(&_controlType, sizeof(_controlType));
	}

};
