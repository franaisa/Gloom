#ifndef __Logic_MessageSpawnIsLive_H
#define __Logic_MessageSpawnIsLive_H

#include "Message.h"

namespace Logic {

	class CMessageSpawnIsLive : public CMessage{
	DEC_FACTORYMESSAGE(CMessageSpawnIsLive);
	public:
		CMessageSpawnIsLive();
		virtual ~CMessageSpawnIsLive(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	};
	REG_FACTORYMESSAGE(CMessageSpawnIsLive);
};

#endif