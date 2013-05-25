#include "MessageUntouched.h"
#include "../Entity/Entity.h"

#include "Logic/Entity/MessageFactory.h"
#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageUntouched);

	CMessageUntouched::CMessageUntouched() : CMessage(Message::UNTOUCHED) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	CEntity* CMessageUntouched::getEntity(){
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageUntouched::setEntity(CEntity *c){
		 _entity=c;
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessageUntouched::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_entity->getEntityID()));
		buffer.serialize(std::string("CMessageUntouched"),true);
		buffer.serialize(_entity->getEntityID());
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageUntouched::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
        buffer.deserialize(id);
	}
};