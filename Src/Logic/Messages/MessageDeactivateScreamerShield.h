#ifndef __Logic_MessageDeactivateScreamerShield_H
#define __Logic_MessageDeactivateScreamerShield_H

#include "Message.h"

namespace Logic {

	class CMessageDeactivateScreamerShield : public CMessage{
	DEC_FACTORYMESSAGE(CMessageDeactivateScreamerShield);
	public:
		CMessageDeactivateScreamerShield();
		virtual ~CMessageDeactivateScreamerShield();
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};
	REG_FACTORYMESSAGE(CMessageDeactivateScreamerShield);
};

#endif