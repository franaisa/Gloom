#include "MessageUntouched.h"

namespace Logic {

	CMessageUntouched::CMessageUntouched(TMessageType type) : CMessage(type) {
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
		CMessage::serialize();
		_tempBuffer.write(_entity, sizeof(_entity));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
