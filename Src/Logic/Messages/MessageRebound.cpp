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

	Net::CBuffer* CMessageRebound::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(float) * 3);
		_tempBuffer->serialize(std::string("CMessageRebound"),true);
		_tempBuffer->serialize(_dir);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageRebound::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_dir);
	}

};
