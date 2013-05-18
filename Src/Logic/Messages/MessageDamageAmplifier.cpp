#include "MessageDamageAmplifier.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDamageAmplifier);

	CMessageDamageAmplifier::CMessageDamageAmplifier() : CMessage(Message::DAMAGE_AMPLIFIER) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageDamageAmplifier::setPercentDamage(int percentDamage){
		_percentDamage = percentDamage;
	}//
	//----------------------------------------------------------

	int CMessageDamageAmplifier::getPercentDamage(){
		return _percentDamage;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageDamageAmplifier::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(2*sizeof(int));
		_tempBuffer->serialize(std::string("CMessageDamageAmplifier"),true);
		_tempBuffer->serialize(_percentDamage);

		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageDamageAmplifier::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_percentDamage);
	}

};
