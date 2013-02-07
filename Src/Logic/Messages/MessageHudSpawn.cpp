#include "MessageHudSpawn.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudSpawn);

	CMessageHudSpawn::CMessageHudSpawn() : CMessage(TMessageType::HUD_SPAWN) {
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
		
	Net::CBuffer* CMessageHudSpawn::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_time));
		_tempBuffer->serialize(std::string("CMessageHudSpawn"));
		_tempBuffer->serialize(_time);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudSpawn::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_time);
	}

};
