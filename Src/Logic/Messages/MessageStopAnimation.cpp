#include "MessageStopAnimation.h"

namespace Logic {

	CMessageStopAnimation::CMessageStopAnimation(TMessageType type = TMessageType::STOP_ANIMATION) : CMessage(type) {
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

	Net::CBuffer CMessageStopAnimation::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_string, sizeof(_string));
		_tempBuffer.write(&_bool, sizeof(_bool));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};