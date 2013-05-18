#ifndef __Logic_MessageReducedCooldown_H
#define __Logic_MessageReducedCooldown_H

#include "Message.h"

namespace Logic {

	class CMessageReducedCooldown: public CMessage{
	DEC_FACTORYMESSAGE(CMessageReducedCooldown);
	public:
		CMessageReducedCooldown();
		int getPercentCooldown();
		void setPercentCooldown(int percentCooldown);
		~CMessageReducedCooldown(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _percentCooldown;
	};
	REG_FACTORYMESSAGE(CMessageReducedCooldown);
};

#endif