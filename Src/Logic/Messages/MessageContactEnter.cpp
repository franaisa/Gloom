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
	
	Physics::CContactPoint CMessageContactEnter::getContactPoint() {
		return _contactPoint;
	}

	void CMessageContactEnter::setEntity(CEntity* entity){
		 this->_entity = entity;
	}//
	//----------------------------------------------------------

	void CMessageContactEnter::setContactPoint(const Physics::CContactPoint& contactPoint) {
		_contactPoint.impulse = contactPoint.impulse;
		_contactPoint.normal = contactPoint.normal;
		_contactPoint.position = contactPoint.position;
		_contactPoint.separation = contactPoint.separation;
	}

	Net::CBuffer CMessageContactEnter::serialize() {
		Net::CBuffer buffer(sizeof(int)*2);
		buffer.serialize(std::string("CMessageContactEnter"), true);
		
		TEntityID id = _entity->getEntityID();
		buffer.write( &id, sizeof(id) );

		// No serializo el punto de contacto por no ser una informacion relevante
		// para el cliente

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageContactEnter::deserialize(Net::CBuffer& buffer) {
		TEntityID id;

        buffer.deserialize(id);
		_entity = CServer::getSingletonPtr()->getMap()->getEntityByID(id);

		// No deserializo el punto de contacto por que es una informacion
		// que no es relevante para el cliente
	}

};
