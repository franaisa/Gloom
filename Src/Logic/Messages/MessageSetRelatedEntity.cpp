#include "MessageSetRelatedEntity.h"

#include "Logic/Entity/MessageFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetRelatedEntity);

	CMessageSetRelatedEntity::CMessageSetRelatedEntity() : CMessage(Message::SET_RELATED_ENTITY) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	CMessageSetRelatedEntity::~CMessageSetRelatedEntity() {
		// Nada que hacer
	}
	//----------------------------------------------------------

	CEntity* CMessageSetRelatedEntity::getRelatedEntity() {
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageSetRelatedEntity::setRelatedEntity(CEntity* entity) {
		this->_entity = entity;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageSetRelatedEntity::serialize() {
		assert(_tempBuffer == NULL);

		Logic::TEntityID id = _entity->getEntityID();

		_tempBuffer = new Net::CBuffer( sizeof(int) + sizeof(id) );
		_tempBuffer->serialize(std::string("CMessageSetRelatedEntity"), true);
		_tempBuffer->serialize(id);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSetRelatedEntity::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
		// Por problemas con enumerados utilizamos directamente
		// el read en vez del deserialize
		buffer.read( &id, sizeof(id) );

		_entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	}

};
