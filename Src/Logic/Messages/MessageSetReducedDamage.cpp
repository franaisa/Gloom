#include "MessageSetReducedDamage.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetReducedDamage);

	//__________________________________________________________________

	CMessageSetReducedDamage::CMessageSetReducedDamage() : CMessage(Message::SET_REDUCED_DAMAGE) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMessageSetReducedDamage::~CMessageSetReducedDamage() {
		// Nada que hacer
	}

	//__________________________________________________________________

	float CMessageSetReducedDamage::getReducedDamage() {
		return _reducedDamage;
	}

	//__________________________________________________________________

	void CMessageSetReducedDamage::setReducedDamage(float percentage){
		_reducedDamage = percentage;
	}

	//__________________________________________________________________
		
	Net::CBuffer CMessageSetReducedDamage::serialize() {
		// Tamaño de la cabecera (int) + porcentaje de reducción (float)
		Net::CBuffer buffer( sizeof(int) + sizeof(float) );
		buffer.serialize( std::string("CMessageSetReducedDamage"), true );
		buffer.serialize(_reducedDamage);
		
		return buffer;
	}
	
	//__________________________________________________________________

	void CMessageSetReducedDamage::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_reducedDamage);
	}

};
