#include "MessageTouched.h"
#include "../Entity/Entity.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageTouched);

	CMessageTouched::CMessageTouched() : CMessage(TMessageType::TOUCHED) {
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

	Net::CBuffer* CMessageTouched::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_entity->getEntityID()));
		_tempBuffer->serialize(std::string("CMessageTouched"));
		_tempBuffer->serialize(_entity->getEntityID());
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageTouched::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
        buffer.deserialize(id);
	}

};
