#ifndef __Logic_MessageHudLife_H
#define __Logic_MessageHudLife_H

#include "Message.h"

namespace Logic {

	class CMessageHudLife: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudLife);
	public:
		CMessageHudLife();
		int getHudLife();
		void setHudLife( int life);
		~CMessageHudLife(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _life;
	};
	REG_FACTORYMESSAGE(CMessageHudLife);
};

#endif