#ifndef __Logic_MessageElevatorFinal_H
#define __Logic_MessageElevatorFinal_H

#include "Message.h"

namespace Logic {

	class CMessageElevatorFinal : public CMessage{
	DEC_FACTORYMESSAGE(CMessageElevatorFinal);
	public:
		CMessageElevatorFinal();
		~CMessageElevatorFinal(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};
	REG_FACTORYMESSAGE(CMessageElevatorFinal);
};

#endif