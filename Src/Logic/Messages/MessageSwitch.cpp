#include "MessageSwitch.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSwitch);

	CMessageSwitch::CMessageSwitch() : CMessage(Message::SWITCH) {
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
		Net::CBuffer buffer(sizeof(int) + sizeof(unsigned char));
		buffer.serialize(std::string("CMessageSwitch"),true);
		buffer.serialize(_change);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSwitch::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_change);
	}
};
