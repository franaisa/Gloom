#include "MessageRebound.h"

#include <string>

namespace Logic {
	IMP_FACTORYMESSAGE(CMessageRebound);

	CMessageRebound::CMessageRebound() : CMessage(Message::REBOUND) {
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
		Net::CBuffer buffer(sizeof(int) + sizeof(float) * 3);
		buffer.serialize(std::string("CMessageRebound"),true);
		buffer.serialize(_dir);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageRebound::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_dir);
	}

};
