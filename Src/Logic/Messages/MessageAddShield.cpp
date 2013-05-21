#include "MessageAddShield.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddShield);

	CMessageAddShield::CMessageAddShield() : CMessage(Message::ADD_SHIELD) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAddShield::setAddShield( int shield){
		_shield = shield;
	}//
	//----------------------------------------------------------

	int CMessageAddShield::getAddShield(){
		return _shield;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageAddShield::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_shield));
		buffer.serialize(std::string("CMessageAddShield"),true);
		buffer.serialize(_shield);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAddShield::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_shield);
	}

};
