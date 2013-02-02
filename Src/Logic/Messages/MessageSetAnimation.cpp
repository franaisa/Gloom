#include "MessageSetAnimation.h"

namespace Logic {

	CMessageSetAnimation::CMessageSetAnimation(TMessageType type = TMessageType::SET_ANIMATION) : CMessage(type) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	std::string CMessageSetAnimation::getString(){
		return _string;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::setString(std::string string){
		_string=string;
	}//
	//----------------------------------------------------------

	bool CMessageSetAnimation::getBool(){
		return _bool;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::setBool(bool boolean){
		_bool=boolean;
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessageSetAnimation::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_string, sizeof(_string));
		_tempBuffer.write(&_bool, sizeof(_bool));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
