#ifndef __Logic_MessageJumper_H
#define __Logic_MessageJumper_H

#include "Message.h"

namespace Logic {

	class CMessageJumper : public CMessage{
	public:
		CMessageJumper();
		void setPower(float power);
		float getPower();
		~CMessageJumper(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	protected:
		float _power;
	};

};

#endif