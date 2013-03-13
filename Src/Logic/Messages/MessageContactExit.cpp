#include "MessageContactExit.h"
#include "../Entity/Entity.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageContactExit);

	CMessageContactExit::CMessageContactExit() : CMessage(Message::CONTACT_EXIT) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	CEntity* CMessageContactExit::getEntity(){
		return _entity;
	}//
	//----------------------------------------------------------
	
	void CMessageContactExit::setEntity(CEntity *c){
		 _entity=c;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageContactExit::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_entity->getEntityID()));
		_tempBuffer->serialize(std::string("CMessageContactExit"),true);
		_tempBuffer->serialize(_entity->getEntityID());
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageContactExit::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
        buffer.deserialize(id);
	}

};
