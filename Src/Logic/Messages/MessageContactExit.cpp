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

	Net::CBuffer CMessageContactExit::serialize() {
		Net::CBuffer buffer(sizeof(int)*2);
		buffer.serialize(std::string("CMessageContactExit"),true);
		buffer.serialize(_entity);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageContactExit::deserialize(Net::CBuffer& buffer) {
        buffer.deserialize(_entity);
	}

};
