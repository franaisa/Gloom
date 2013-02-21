#ifndef __Logic_MessageSide_H
#define __Logic_MessageSide_H

#include "Message.h"

namespace Logic {

	class CMessageSide : public CMessage{
	DEC_FACTORYMESSAGE(CMessageSide);
	public:
		CMessageSide();
		~CMessageSide(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};
	REG_FACTORYMESSAGE(CMessageSide);
};

#endif