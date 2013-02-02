#include "MessageRebound.h"

namespace Logic {

	CMessageRebound::CMessageRebound(TMessageType type = TMessageType::REBOUND) : CMessage(type) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Vector3 CMessageRebound::getDir(){
		return _dir;
	}//
	//----------------------------------------------------------

	void CMessageRebound::setDir(Vector3 dir){
		 _dir=dir;
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessageRebound::serialize() {
		CMessage::serialize();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
