#ifndef __Logic_MessageCealing_H
#define __Logic_MessageCealing_H

#include "Message.h"

namespace Logic {

	class CMessageCealing : public CMessage{
	DEC_FACTORYMESSAGE(CMessageCealing);
	public:
		CMessageCealing();
		~CMessageCealing(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};
	REG_FACTORYMESSAGE(CMessageCealing);
};

#endif