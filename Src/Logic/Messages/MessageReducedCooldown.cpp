#include "MessageReducedCooldown.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageReducedCooldown);

	CMessageReducedCooldown::CMessageReducedCooldown() : CMessage(Message::REDUCED_COOLDOWN) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageReducedCooldown::setPercentCooldown(int percentCooldown){
		_percentCooldown = percentCooldown;
	}//
	//----------------------------------------------------------

	int CMessageReducedCooldown::getPercentCooldown(){
		return _percentCooldown;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer* CMessageReducedCooldown::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(2*sizeof(int));
		_tempBuffer->serialize(std::string("CMessageReducedCooldown"),true);
		_tempBuffer->serialize(_percentCooldown);

		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageReducedCooldown::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_percentCooldown);
	}

};
