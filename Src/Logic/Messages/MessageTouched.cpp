#include "MessageTouched.h"

namespace Logic {

	CMessageTouched::CMessageTouched(TMessageType type) : CMessage(type) {
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

		_tempBuffer = new Net::CBuffer(sizeof(_entity->getEntityID()));
		_tempBuffer->serialize(_entity->getEntityID());
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
