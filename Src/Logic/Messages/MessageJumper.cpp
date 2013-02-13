#include "MessageJumper.h"

#include <string>

namespace Logic {

	CMessageJumper::CMessageJumper() : CMessage(TMessageType::JUMPER) {
		// Nada que hacer
	} //
	//----------------------------------------------------------
	float CMessageJumper::getPower(){
		return _power;
	}//
	//----------------------------------------------------------

	void CMessageJumper::setPower(float power){
		_power=power;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageJumper::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageJumper"));
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageJumper::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
