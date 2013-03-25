#ifndef __Logic_MessageUsePrimarySkill_H
#define __Logic_MessageUsePrimarySkill_H

#include "Message.h"

namespace Logic {

	class CMessageUsePrimarySkill: public CMessage{
	DEC_FACTORYMESSAGE(CMessageUsePrimarySkill);
	public:

		CMessageUsePrimarySkill();
		virtual ~CMessageUsePrimarySkill();

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	};
	REG_FACTORYMESSAGE(CMessageUsePrimarySkill);
};

#endif
