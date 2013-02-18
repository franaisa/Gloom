#include "MessageSetAnimation.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

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

		_tempBuffer = new Net::CBuffer((sizeof(int) * 2) + sizeof(bool) + sizeof(unsigned int));
		_tempBuffer->serialize(std::string("CMessageSetAnimation"),true);
		_tempBuffer->serialize(_bool);
		_tempBuffer->serialize(_string, false);
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::deserialize(Net::CBuffer& buffer) {
		unsigned int size;
		buffer.deserialize(_bool);
		buffer.deserialize(_string);
	}

};
