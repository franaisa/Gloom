#ifndef __Logic_MessageActivate_H
#define __Logic_MessageActivate_H

#include "Message.h"

namespace Logic {

	class CMessageActivate : public CMessage{
	public:
		CMessageActivate();
		~CMessageActivate(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};

};

#endif