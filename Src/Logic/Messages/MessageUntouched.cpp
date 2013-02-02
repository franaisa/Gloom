#include "MessageUntouched.h"

namespace Logic {

	CMessageUntouched::CMessageUntouched(TMessageType type = TMessageType::UNTOUCHED) : CMessage(type) {
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

		//_tempBuffer = new Net::CBuffer(sizeof(_entity->getEntityID()));
		//_tempBuffer->serialize(_entity->getEntityID());
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
