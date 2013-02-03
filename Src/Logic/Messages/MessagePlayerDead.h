#ifndef __Logic_MessagePlayerDead_H
#define __Logic_MessagePlayerDead_H

#include "Message.h"

namespace Logic {

	class CMessagePlayerDead: public CMessage{
	DEC_FACTORYMESSAGE(CMessagePlayerDead);
	public:
		CMessagePlayerDead();
		bool getPlayerDead();	
		~CMessagePlayerDead(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _playerDead;
	};
	REG_FACTORYMESSAGE(CMessagePlayerDead);
};

#endif