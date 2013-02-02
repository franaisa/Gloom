#include "MessageSetAnimation.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetAnimation);

	CMessageSetAnimation::CMessageSetAnimation() : CMessage(TMessageType::SET_ANIMATION) {
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

	Net::CBuffer* CMessageSetAnimation::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(bool));
		_tempBuffer->serialize(_string);
		_tempBuffer->serialize(_bool);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
