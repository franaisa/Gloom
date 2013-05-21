#include "MessageStopAnimation.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageStopAnimation);

	CMessageStopAnimation::CMessageStopAnimation() : CMessage(Message::STOP_ANIMATION) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	std::string CMessageStopAnimation::getString(){
		return _string;
	}//
	//----------------------------------------------------------

	void CMessageStopAnimation::setString(std::string string){
		_string=string;
	}//

	//----------------------------------------------------------

	Net::CBuffer CMessageStopAnimation::serialize() {
		Net::CBuffer buffer((sizeof(int) * 2) + sizeof(bool));
		buffer.serialize(std::string("CMessageStopAnimation"),true);
		buffer.serialize(_string, false);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageStopAnimation::deserialize(Net::CBuffer& buffer) {
		//unsigned int size;
		buffer.deserialize(_string);
	}

};