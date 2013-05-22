#include "MessageHudLife.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudLife);

	CMessageHudLife::CMessageHudLife() : CMessage(Message::HUD_LIFE) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageHudLife::setLife( int life){
		_life = life;
	}//
	//----------------------------------------------------------

	int CMessageHudLife::getLife(){
		return _life;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageHudLife::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_life));
		buffer.serialize(std::string("CMessageHudLife"),true);
		buffer.serialize(_life);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudLife::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_life);
	}

};
