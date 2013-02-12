#ifndef __Logic_MessageJumper_H
#define __Logic_MessageJumper_H

#include "Message.h"

namespace Logic {

	class CMessageJumper : public CMessage{
	public:
		CMessageJumper();
		~CMessageJumper(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	};

};

#endif