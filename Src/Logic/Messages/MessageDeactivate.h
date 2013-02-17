#ifndef __Logic_MessageDeactivate_H
#define __Logic_MessageDeactivate_H

#include "Message.h"

namespace Logic {

	class CMessageDeactivate : public CMessage{
	public:
		CMessageDeactivate();
		~CMessageDeactivate(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};

};

#endif