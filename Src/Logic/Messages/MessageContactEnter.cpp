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

	Net::CBuffer CMessageContactEnter::serialize() {
		Net::CBuffer buffer(sizeof(int)*2);
		buffer.serialize(std::string("CMessageContactEnter"),true);
		buffer.serialize(_entity);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageContactEnter::deserialize(Net::CBuffer& buffer) {
        buffer.deserialize(_entity);
	}

};
