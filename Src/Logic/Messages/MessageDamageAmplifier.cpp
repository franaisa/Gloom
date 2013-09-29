#include "MessageDamageAmplifier.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDamageAmplifier);

	CMessageDamageAmplifier::CMessageDamageAmplifier() : CMessage(Message::DAMAGE_AMPLIFIER), _duration(0) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	CMessageDamageAmplifier::CMessageDamageAmplifier(int percentDamage) : CMessage(Message::DAMAGE_AMPLIFIER), _percentDamage(percentDamage) {
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
		
	Net::CBuffer CMessageDamageAmplifier::serialize() {

		Net::CBuffer buffer(2*sizeof(int));
		buffer.serialize(std::string("CMessageDamageAmplifier"),true);
		buffer.serialize(_percentDamage);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageDamageAmplifier::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_percentDamage);
	}

};
