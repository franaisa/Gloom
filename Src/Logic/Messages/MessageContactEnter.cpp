#include "MessageContactEnter.h"
#include "../Entity/Entity.h"

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
	
	void CMessageContactEnter::setEntity(CEntity *c){
		 _entity=c;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageContactEnter::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_entity->getEntityID()));
		_tempBuffer->serialize(std::string("CMessageContactEnter"),true);
		_tempBuffer->serialize(_entity->getEntityID());
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageContactEnter::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
        buffer.deserialize(id);
	}

};
