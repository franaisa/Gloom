#include "MessageSyncPosition.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSyncPosition);
	
	CMessageSyncPosition::CMessageSyncPosition() : CMessage(Message::SYNC_POSITION) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageSyncPosition::setPosition(const Vector3& position) {
		_position = position;
	}//
	//----------------------------------------------------------

	void CMessageSyncPosition::setSeq(const unsigned int time) { 
		_seq = time;
	}
	//----------------------------------------------------------
	Vector3 CMessageSyncPosition::getPosition(){
		return _position;
	}//
	//----------------------------------------------------------

	unsigned int CMessageSyncPosition::getSeq(){
		return _seq;
	}//

	//----------------------------------------------------------
	Net::CBuffer CMessageSyncPosition::serialize() {
		// El tamaño de 3 floats para el Vector3 de direccion y 
		// 2 floats para la orientacion (yaw y pitch)
		Net::CBuffer buffer( sizeof(int) + (sizeof(float) * 5) );
		buffer.serialize(std::string("CMessageSyncPosition"), true);
		buffer.serialize(_position);
		buffer.serialize(_seq);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSyncPosition::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_position);
		buffer.deserialize(_seq);
	}
};
