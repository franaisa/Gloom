#include "MessageContactEnter.h"
#include "../Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageContactEnter);

	CMessageContactEnter::CMessageContactEnter() : CMessage(Message::CONTACT_ENTER) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	CEntity* CMessageContactEnter::getEntity(){
		return _entity;
	}//
	//----------------------------------------------------------
	
	void CMessageContactEnter::setEntity(CEntity* entity){
		 this->_entity = entity;
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessageContactEnter::serialize() {
		Net::CBuffer buffer(sizeof(int)*2);
		buffer.serialize(std::string("CMessageContactEnter"), true);
		
		TEntityID id = _entity->getEntityID();
		buffer.write( &id, sizeof(id) );

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageContactEnter::deserialize(Net::CBuffer& buffer) {
		TEntityID id;

        buffer.deserialize(id);
		_entity = CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	}

};
