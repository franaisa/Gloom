#ifndef __Logic_MessageHudShield_H
#define __Logic_MessageHudShield_H

#include "Message.h"

namespace Logic {

	class CMessageHudShield: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudShield);
	public:
		CMessageHudShield();
		int getShield();
		void setShield( int shield);
		~CMessageHudShield(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _shield;
	};
	REG_FACTORYMESSAGE(CMessageHudShield);
};

#endif