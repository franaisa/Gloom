#ifndef __Logic_MessageActivateScreamerShield_H
#define __Logic_MessageActivateScreamerShield_H

#include "Message.h"

namespace Logic {

	class CMessageActivateScreamerShield : public CMessage{
	DEC_FACTORYMESSAGE(CMessageActivateScreamerShield);
	public:
		CMessageActivateScreamerShield();
		virtual ~CMessageActivateScreamerShield();
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};
	REG_FACTORYMESSAGE(CMessageActivateScreamerShield);
};

#endif