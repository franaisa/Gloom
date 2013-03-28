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
		
	Net::CBuffer* CMessageSetReducedDamage::serialize() {
		assert(_tempBuffer == NULL);

		// Tamaño de la cabecera (int) + porcentaje de reducción (float)
		_tempBuffer = new Net::CBuffer( sizeof(int) + sizeof(float) );
		_tempBuffer->serialize( std::string("CMessageSetReducedDamage"), true );
		_tempBuffer->serialize(_reducedDamage);
		
		return _tempBuffer;
	}
	
	//__________________________________________________________________

	void CMessageSetReducedDamage::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_reducedDamage);
	}

};
