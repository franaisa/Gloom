#ifndef __Logic_MessageUseSecondarySkill_H
#define __Logic_MessageUseSecondarySkill_H

#include "Message.h"

namespace Logic {

	class CMessageUseSecondarySkill: public CMessage{
	DEC_FACTORYMESSAGE(CMessageUseSecondarySkill);
	public:

		CMessageUseSecondarySkill();
		virtual ~CMessageUseSecondarySkill();

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	};
	REG_FACTORYMESSAGE(CMessageUseSecondarySkill);
};

#endif
