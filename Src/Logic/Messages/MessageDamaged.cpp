#include "MessageDamaged.h"

namespace Logic {

	CMessageDamaged::CMessageDamaged(TMessageType type = TMessageType::DAMAGED) : CMessage(type) {
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
		
	Net::CBuffer CMessageDamaged::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_damage, sizeof(_damage));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
