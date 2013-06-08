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
	Vector3 CMessageSyncPosition::getPosition(){
		return _position;
	}//
	//----------------------------------------------------------

	void CMessageSyncPosition::setOrientation(const Quaternion& orientation) {
		_orientation = orientation;
	}//
	//----------------------------------------------------------
	Quaternion CMessageSyncPosition::getOrientation(){
		return _orientation;
	}//
	//----------------------------------------------------------


	Net::CBuffer CMessageSyncPosition::serialize() {
		// El tamaño de 3 floats para el Vector3 de direccion y 
		// 3 floats para la orientacion (yaw,pitch,roll)
		Net::CBuffer buffer( sizeof(int) + (sizeof(float)*3) );
		buffer.serialize(std::string("CMessageSyncPosition"), true);
		buffer.serialize(_position);
		buffer.serialize(_orientation);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSyncPosition::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_position);
		buffer.deserialize(_orientation);
	}
};
