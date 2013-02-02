#include "MessageControl.h"

#include "Logic/Entity/MessageFactory.h"

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

		_tempBuffer = new Net::CBuffer(sizeof(_controlType));
		_tempBuffer->serialize(_controlType);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageControl::deserialize(Net::CBuffer& buffer) {
		// NO EXISTE FUNCION QUE HAGA MATCH CON UN TIPO ENUM
		// HAY QUE IMPLEMENTARSE UN DESERIALIZE DE UNSIGNED INT??
		//buffer.deserialize(_controlType);
	}

};
