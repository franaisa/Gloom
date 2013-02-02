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

	Net::CBuffer* CMessageStopAnimation::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(bool));
		_tempBuffer->serialize(_string);
		_tempBuffer->serialize(_bool);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};