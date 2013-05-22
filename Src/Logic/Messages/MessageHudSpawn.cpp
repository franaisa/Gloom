#include "MessageHudSpawn.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudSpawn);

	CMessageHudSpawn::CMessageHudSpawn() : CMessage(Message::HUD_SPAWN) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageHudSpawn::setTime( int time){
		_time = time;
	}//
	//----------------------------------------------------------

	int CMessageHudSpawn::getTime(){
		return _time;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageHudSpawn::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_time));
		buffer.serialize(std::string("CMessageHudSpawn"),true);
		buffer.serialize(_time);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudSpawn::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_time);
	}

};
