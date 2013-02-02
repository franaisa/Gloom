#include "MessageControl.h"

namespace Logic {

	CMessageControl::CMessageControl(TMessageType type = TMessageType::CONTROL) : CMessage(type) {
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

	Net::CBuffer CMessageControl::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_controlType, sizeof(_controlType));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
