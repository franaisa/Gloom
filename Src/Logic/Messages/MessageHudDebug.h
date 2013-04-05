#ifndef __Logic_MessageHudDebug_H
#define __Logic_MessageHudDebug_H

#include "Message.h"

namespace Logic {

	class CMessageHudDebug: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudDebug);
	public:
		CMessageHudDebug();
		~CMessageHudDebug(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
	};
	REG_FACTORYMESSAGE(CMessageHudDebug);
};

#endif