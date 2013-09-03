#ifndef __Logic_MessageKillStreak_H
#define __Logic_MessageKillStreak_H

#include "Message.h"

namespace Logic {

	class CMessageKillStreak: public CMessage{
	DEC_FACTORYMESSAGE(CMessageKillStreak);
	public:
		CMessageKillStreak();
		virtual ~CMessageKillStreak(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setKiller(Logic::TEntityID killer)		{_killer = killer;}
		Logic::TEntityID getKiller()				{return _killer;}

	private:
		Logic::TEntityID _killer;

	};
	REG_FACTORYMESSAGE(CMessageKillStreak);
};

#endif