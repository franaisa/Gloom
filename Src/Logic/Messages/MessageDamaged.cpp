#include "MessageDamaged.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDamaged);

	CMessageDamaged::CMessageDamaged() : CMessage(TMessageType::DAMAGED) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageDamaged::setDamage(unsigned char damage){
		_damage = damage;
	}//
	//----------------------------------------------------------

	unsigned char CMessageDamaged::getDamage(){
		return _damage;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageDamaged::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(_damage));
		_tempBuffer->serialize(_damage);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
