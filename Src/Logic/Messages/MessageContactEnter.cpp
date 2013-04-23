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

	Logic::TEntityID CMessageContactEnter::getEntity(){
		return _entity;
	}//
	//----------------------------------------------------------
	
	void CMessageContactEnter::setEntity(Logic::TEntityID c){
		 _entity=c;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageContactEnter::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int)*2);
		_tempBuffer->serialize(std::string("CMessageContactEnter"),true);
		_tempBuffer->serialize(_entity);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageContactEnter::deserialize(Net::CBuffer& buffer) {
        buffer.deserialize(_entity);
	}

};
