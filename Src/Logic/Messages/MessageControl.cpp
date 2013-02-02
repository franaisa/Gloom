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

	Net::CBuffer* CMessageControl::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(_controlType));
		_tempBuffer->serialize(_controlType);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
