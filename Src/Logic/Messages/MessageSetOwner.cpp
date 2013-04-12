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
		return _owner;
	}//
	//----------------------------------------------------------

	void CMessageSetOwner::setOwner(CEntity* entity) {
		_owner = entity;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageSetOwner::serialize() {
		assert(_tempBuffer == NULL);

		Logic::TEntityID id = _owner->getEntityID();

		_tempBuffer = new Net::CBuffer( sizeof(int) + sizeof(id) );
		_tempBuffer->serialize(std::string("CMessageSetOwner"), true);
		_tempBuffer->serialize(id);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSetOwner::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
		// Por problemas con enumerados utilizamos directamente
		// el read en vez del deserialize
		buffer.read( &id, sizeof(id) );

		_owner = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	}

};
