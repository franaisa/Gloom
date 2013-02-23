#include "MessageElevatorInitial.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageElevatorInitial);

	CMessageElevatorInitial::CMessageElevatorInitial() : CMessage(TMessageType::ELEVATOR_INITIAL) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer* CMessageElevatorInitial::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int));
		_tempBuffer->serialize(std::string("CMessageElevatorInitial"), true);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageElevatorInitial::deserialize(Net::CBuffer& buffer) {
		//Nada
	}

};
