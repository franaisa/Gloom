#ifndef __Logic_MessageDeactivate_H
#define __Logic_MessageDeactivate_H

#include "Message.h"

namespace Logic {

	class CMessageDeactivate : public CMessage{
	DEC_FACTORYMESSAGE(CMessageDeactivate);
	public:
		CMessageDeactivate();
		virtual ~CMessageDeactivate(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};
	REG_FACTORYMESSAGE(CMessageDeactivate);
};

#endif