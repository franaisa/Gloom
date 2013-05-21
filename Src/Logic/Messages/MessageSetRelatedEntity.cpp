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
	
	Net::CBuffer CMessageSetRelatedEntity::serialize() {
		Logic::TEntityID id = _entity->getEntityID();

		Net::CBuffer buffer( sizeof(int) + sizeof(id) );
		buffer.serialize(std::string("CMessageSetRelatedEntity"), true);
		buffer.serialize(id);
		
		return buffer;
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
