#ifndef __Logic_MessageHudDebbug_H
#define __Logic_MessageHudDebbug_H

#include "Message.h"

namespace Logic {

	class CMessageHudDebbug: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudDebbug);
	public:
		CMessageHudDebbug();
		~CMessageHudDebbug(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
	};
	REG_FACTORYMESSAGE(CMessageHudDebbug);
};

#endif