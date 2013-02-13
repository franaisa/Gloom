#ifndef __Logic_MessageSleep_H
#define __Logic_MessageSleep_H

#include "Message.h"

namespace Logic {

	class CMessageSleep : public CMessage{
	public:
		CMessageSleep();
		~CMessageSleep(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};

};

#endif