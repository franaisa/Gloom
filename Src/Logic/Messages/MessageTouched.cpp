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

	Net::CBuffer CMessageTouched::serialize() {
		CMessage::serialize();
		_tempBuffer.write(_entity, sizeof(_entity));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
