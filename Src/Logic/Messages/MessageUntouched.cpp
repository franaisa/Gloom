#include "MessageUntouched.h"
#include "../Entity/Entity.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageUntouched);

	CMessageUntouched::CMessageUntouched() : CMessage(TMessageType::UNTOUCHED) {
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

	Net::CBuffer* CMessageUntouched::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(_entity->getEntityID()));
		_tempBuffer->serialize(_entity->getEntityID());
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageUntouched::deserialize(Net::CBuffer& buffer) {
		int temp;

		buffer.deserialize(temp);
		// HAY QUE CONSTRUIR UNA ENTIDAD ENTITY - NO EXISTE METODO
		// setEntityID
	}
};