#include "MessageStopAnimation.h"


#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageStopAnimation);

	CMessageStopAnimation::CMessageStopAnimation() : CMessage(TMessageType::STOP_ANIMATION) {
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

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(bool));
		_tempBuffer->serialize(_string);
		_tempBuffer->serialize(_bool);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageStopAnimation::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_string);
		buffer.deserialize(_bool);
	}

};