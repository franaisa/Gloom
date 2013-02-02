#ifndef __Logic_MessageSwitch_H
#define __Logic_MessageSwitch_H

#include "Message.h"

namespace Logic {

	class CMessageSwitch: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSwitch);
	public:
		CMessageSwitch();
		unsigned char getChange();
		void setChange(unsigned char change);
		~CMessageSwitch(){};
		
		virtual Net::CBuffer* serialize();

	private:
		unsigned char _change;
	};
	REG_FACTORYMESSAGE(CMessageSwitch);
};

#endif