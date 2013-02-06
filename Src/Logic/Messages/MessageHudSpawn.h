#ifndef __Logic_MessageHudSpawn_H
#define __Logic_MessageHudSpawn_H

#include "Message.h"

namespace Logic {

	class CMessageHudSpawn: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudSpawn);
	public:
		CMessageHudSpawn();
		int getTime();
		void setTime( int time);
		~CMessageHudSpawn(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _time;
	};
	REG_FACTORYMESSAGE(CMessageHudSpawn);
};

#endif