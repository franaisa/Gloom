#include "MessageSetAnimation.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetAnimation);

	CMessageSetAnimation::CMessageSetAnimation() : CMessage(Message::SET_ANIMATION) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	std::string CMessageSetAnimation::getAnimation(){
		return _animation;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::setAnimation(std::string animation){
		_animation=animation;
	}//
	//----------------------------------------------------------

	bool CMessageSetAnimation::getLoop(){
		return _loop;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::setLoop(bool loop){
		_loop=loop;
	}//

	//----------------------------------------------------------

	bool CMessageSetAnimation::getExclude(){
		return _exclude;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::setExclude(bool exclude){
		_exclude = exclude;
	}//

	int CMessageSetAnimation::getRewind(){
		return _rewind;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::setRewind(int rewind){
		_rewind = rewind;
	}//

	//----------------------------------------------------------

	Net::CBuffer CMessageSetAnimation::serialize() {
		Net::CBuffer buffer((sizeof(int) * 2) + sizeof(bool) + sizeof(unsigned int));
		buffer.serialize(std::string("CMessageSetAnimation"),true);
		buffer.serialize(_loop);
		buffer.serialize(_rewind);
		buffer.serialize(_animation, false);
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_loop);
		buffer.deserialize(_rewind);
		buffer.deserialize(_animation);
		
	}

};
