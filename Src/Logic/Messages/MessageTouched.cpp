#include "MessageTouched.h"
#include "../Entity/Entity.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageTouched);

	CMessageTouched::CMessageTouched() : CMessage(Message::TOUCHED) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	CEntity* CMessageTouched::getEntity(){
		return _entity;
	}//
	//----------------------------------------------------------
	
	void CMessageTouched::setEntity(CEntity *c){
		 _entity=c;
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessageTouched::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_entity->getEntityID()));
		buffer.serialize(std::string("CMessageTouched"),true);
		buffer.serialize(_entity->getEntityID());
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageTouched::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
        buffer.deserialize(id);
	}

};
