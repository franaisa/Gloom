#include "MessageSetOwner.h"

#include "Logic/Entity/MessageFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetOwner);

	CMessageSetOwner::CMessageSetOwner() : CMessage(Message::SET_OWNER) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	CMessageSetOwner::~CMessageSetOwner() {
		// Nada que hacer
	}
	//----------------------------------------------------------

	CEntity* CMessageSetOwner::getOwner() {
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageSetOwner::setOwner(CEntity* entity) {
		this->_entity = entity;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer CMessageSetOwner::serialize() {
		Logic::TEntityID id = _entity->getEntityID();

		Net::CBuffer buffer( sizeof(int) + sizeof(id) );
		buffer.serialize(std::string("CMessageSetOwner"), true);
		buffer.serialize(id);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSetOwner::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
		// Por problemas con enumerados utilizamos directamente
		// el read en vez del deserialize
		buffer.read( &id, sizeof(id) );

		_entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	}

};
