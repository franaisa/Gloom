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
	
	Net::CBuffer CMessageSwitch::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_change, sizeof(_change));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
