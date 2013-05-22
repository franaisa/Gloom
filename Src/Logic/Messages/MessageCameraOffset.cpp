#include "MessageCameraOffset.h"

#include "Logic/Entity/MessageFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageCameraOffset);

	CMessageCameraOffset::CMessageCameraOffset() : CMessage(Message::CAMERA_OFFSET) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	CMessageCameraOffset::~CMessageCameraOffset() {
		// Nada que hacer
	}
	//----------------------------------------------------------

	void CMessageCameraOffset::setOffsetTimer(float fOffsetTimer) {
		this->_fOffsetTimer = fOffsetTimer;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer CMessageCameraOffset::serialize() {
		Logic::TEntityID id = _entity->getEntityID();

		Net::CBuffer buffer( sizeof(int) + sizeof(id) );
		buffer.serialize(std::string("CMessageCameraOffset"), true);
		buffer.serialize(id);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageCameraOffset::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
		// Por problemas con enumerados utilizamos directamente
		// el read en vez del deserialize
		buffer.read( &id, sizeof(id) );

		_entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	}

};
