#include "MessageControl.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageControl);

	CMessageControl::CMessageControl( ) : CMessage(Message::CONTROL),
										  _controlType(ControlType::UNASSIGNED),
										  _seq(0) {
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

	void CMessageControl::setSeqNumber(unsigned int seq){
		_seq=seq;
	}

	//----------------------------------------------------------

	unsigned int CMessageControl::getSeqNumber(){
		return _seq;
	}


	//----------------------------------------------------------

	Net::CBuffer CMessageControl::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_controlType));
		buffer.serialize(std::string("CMessageControl"),true);
		buffer.serialize(_seq);
		buffer.serialize(_controlType);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageControl::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_seq);
		buffer.read(&_controlType, sizeof(_controlType));
		
	}

};
