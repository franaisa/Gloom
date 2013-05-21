#ifndef __Logic_MessageHudDispersion_H
#define __Logic_MessageHudDispersion_H

#include "Message.h"

namespace Logic {

	class CMessageHudDispersion : public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudDispersion);
	public:
		CMessageHudDispersion();
		virtual ~CMessageHudDispersion(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};
	REG_FACTORYMESSAGE(CMessageHudDispersion);
};

#endif