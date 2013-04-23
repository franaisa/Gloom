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

	Logic::TEntityID CMessageContactExit::getEntity(){
		return _entity;
	}//
	//----------------------------------------------------------
	
	void CMessageContactExit::setEntity(Logic::TEntityID c){
		 _entity=c;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageContactExit::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int)*2);
		_tempBuffer->serialize(std::string("CMessageContactExit"),true);
		_tempBuffer->serialize(_entity);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageContactExit::deserialize(Net::CBuffer& buffer) {
        buffer.deserialize(_entity);
	}

};
