#ifndef __Logic_MessageWakeUp_H
#define __Logic_MessageWakeUp_H

#include "Message.h"

namespace Logic {

	class CMessageWakeUp : public CMessage{
	public:
		CMessageWakeUp();
		~CMessageWakeUp(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};

};

#endif