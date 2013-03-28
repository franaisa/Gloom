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
		
	Net::CBuffer* CMessageBerserker::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(3*sizeof(int));
		_tempBuffer->serialize(std::string("CMessageBerserker"),true);
		_tempBuffer->serialize(_percentDamage);
		_tempBuffer->serialize(_percentCooldown);

		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageBerserker::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_percentDamage);
		buffer.deserialize(_percentCooldown);
	}

};
