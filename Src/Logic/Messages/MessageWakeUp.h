#ifndef __Logic_MessageWakeUp_H
#define __Logic_MessageWakeUp_H

#include "Message.h"

namespace Logic {

	class CMessageWakeUp : public CMessage{
	DEC_FACTORYMESSAGE(CMessageWakeUp);
	public:
		CMessageWakeUp();
		virtual ~CMessageWakeUp(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	};
	REG_FACTORYMESSAGE(CMessageWakeUp);

};

#endif