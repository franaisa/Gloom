#include "MessageSwitch.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSwitch);

	CMessageSwitch::CMessageSwitch() : CMessage(TMessageType::SWITCH) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageSwitch::setChange(unsigned char change){
		_change=change;
	}//
	//----------------------------------------------------------

	unsigned char CMessageSwitch::getChange(){
		return _change;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageSwitch::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(unsigned char));
		_tempBuffer->serialize(_change);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSwitch::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_change);
	}
};