#include "MessageSwitch.h"

namespace Logic {

	CMessageSwitch::CMessageSwitch(TMessageType type = TMessageType::SWITCH) : CMessage(type) {
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

};
