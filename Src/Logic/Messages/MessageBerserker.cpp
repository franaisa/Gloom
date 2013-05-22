#include "MessageBerserker.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageBerserker);

	CMessageBerserker::CMessageBerserker() : CMessage(Message::BERSERKER) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageBerserker::setPercentDamage(int percentDamage){
		_percentDamage = percentDamage;
	}//
	//----------------------------------------------------------

	int CMessageBerserker::getPercentDamage(){
		return _percentDamage;
	}//
	//----------------------------------------------------------

	void CMessageBerserker::setPercentCooldown(int percentCooldown){
		_percentCooldown = percentCooldown;
	}//
	//----------------------------------------------------------

	int CMessageBerserker::getPercentCooldown(){
		return _percentCooldown;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageBerserker::serialize() {
		Net::CBuffer buffer(3*sizeof(int));
		buffer.serialize(std::string("CMessageBerserker"),true);
		buffer.serialize(_percentDamage);
		buffer.serialize(_percentCooldown);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageBerserker::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_percentDamage);
		buffer.deserialize(_percentCooldown);
	}

};
