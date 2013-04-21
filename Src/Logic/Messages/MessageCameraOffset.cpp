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

	CEntity* CMessageCameraOffset::getRelatedEntity() {
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageCameraOffset::setRelatedEntity(CEntity* entity) {
		this->_entity = entity;
	}//
	//----------------------------------------------------------

	void CMessageCameraOffset::setOffset(float fOffset) {
		this->_fOffset = fOffset;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageCameraOffset::serialize() {
		assert(_tempBuffer == NULL);

		Logic::TEntityID id = _entity->getEntityID();

		_tempBuffer = new Net::CBuffer( sizeof(int) + sizeof(id) );
		_tempBuffer->serialize(std::string("CMessageSetRelatedEntity"), true);
		_tempBuffer->serialize(id);
		
		return _tempBuffer;
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
