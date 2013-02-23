#include "MessageElevatorFinal.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageElevatorFinal);

	CMessageElevatorFinal::CMessageElevatorFinal() : CMessage(TMessageType::ELEVATOR_FINAL) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageElevatorFinal::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageElevatorFinal"), true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageElevatorFinal::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
