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

	bool CMessageStopAnimation::getBool(){
		return _bool;
	}//
	//----------------------------------------------------------

	void CMessageStopAnimation::setBool(bool boolean){
		_bool=boolean;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageStopAnimation::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer((sizeof(int) * 2) + sizeof(bool));
		_tempBuffer->serialize(std::string("CMessageStopAnimation"),true);
		_tempBuffer->serialize(_bool);
		_tempBuffer->serialize(_string, false);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageStopAnimation::deserialize(Net::CBuffer& buffer) {
		//unsigned int size;
		buffer.deserialize(_bool);
		buffer.deserialize(_string);
		buffer.deserialize(_bool);
	}

};