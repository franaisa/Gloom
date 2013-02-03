#ifndef __Logic_MessageAddLife_H
#define __Logic_MessageAddLife_H

#include "Message.h"

namespace Logic {

	class CMessageAddLife: public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddLife);
	public:
		CMessageAddLife();
		int getAddLife();
		void setAddLife( int life);
		~CMessageAddLife(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _life;
	};
	REG_FACTORYMESSAGE(CMessageAddLife);
};

#endif