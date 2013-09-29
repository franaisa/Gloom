#ifndef __Logic_MessageReducedCooldown_H
#define __Logic_MessageReducedCooldown_H

#include "Message.h"

namespace Logic {

	class CMessageReducedCooldown: public CMessage{
	DEC_FACTORYMESSAGE(CMessageReducedCooldown);
	public:
		CMessageReducedCooldown();
		CMessageReducedCooldown(int percentCooldown);
		int getPercentCooldown();
		void setPercentCooldown(int percentCooldown);
		void setDuration(unsigned int msecs) { _duration = msecs; }
		unsigned int getDuration() { return _duration; }
		~CMessageReducedCooldown(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _percentCooldown;
		 unsigned int _duration;
	};
	REG_FACTORYMESSAGE(CMessageReducedCooldown);
};

#endif