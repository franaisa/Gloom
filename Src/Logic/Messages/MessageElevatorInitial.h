#ifndef __Logic_MessageElevatorInitial_H
#define __Logic_MessageElevatorInitial_H

#include "Message.h"

namespace Logic {

	class CMessageElevatorInitial : public CMessage{
	DEC_FACTORYMESSAGE(CMessageElevatorInitial);
	public:
		CMessageElevatorInitial();
		~CMessageElevatorInitial(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};
	REG_FACTORYMESSAGE(CMessageElevatorInitial);
};

#endif