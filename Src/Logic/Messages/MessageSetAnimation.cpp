#include "MessageSetAnimation.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetAnimation);

	CMessageSetAnimation::CMessageSetAnimation() : CMessage(Message::SET_ANIMATION) {
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
		Net::CBuffer buffer((sizeof(int) * 2) + sizeof(bool) + sizeof(unsigned int));
		buffer.serialize(std::string("CMessageSetAnimation"),true);
		buffer.serialize(_bool);
		buffer.serialize(_string, false);
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_bool);
		buffer.deserialize(_string);
	}

};
