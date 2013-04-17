#ifndef __Logic_MessagePlayerDead_H
#define __Logic_MessagePlayerDead_H

#include "Message.h"

namespace Logic {

	class CMessagePlayerDead: public CMessage{
	DEC_FACTORYMESSAGE(CMessagePlayerDead);
	public:
		CMessagePlayerDead();
		~CMessagePlayerDead(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setKiller(Logic::TEntityID killer){_killer = killer;}
		Logic::TEntityID getKiller(){return _killer;}

	private:
		Logic::TEntityID _killer;

	};
	REG_FACTORYMESSAGE(CMessagePlayerDead);
};

#endif