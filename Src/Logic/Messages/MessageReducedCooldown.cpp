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
		
	Net::CBuffer CMessageReducedCooldown::serialize() {

		Net::CBuffer buffer(2*sizeof(int));
		buffer.serialize(std::string("CMessageReducedCooldown"),true);
		buffer.serialize(_percentCooldown);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageReducedCooldown::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_percentCooldown);
	}

};
