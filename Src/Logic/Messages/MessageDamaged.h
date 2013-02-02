#ifndef __Logic_MessageDamaged_H
#define __Logic_MessageDamaged_H

#include "Message.h"

namespace Logic {

	class CMessageDamaged: public CMessage{
	public:
		CMessageDamaged(TMessageType t);
		unsigned char getDamage();
		void setDamage(unsigned char damage);
		~CMessageDamaged(){};
		
		virtual Net::CBuffer* serialize();

	private:
		unsigned char _damage;
	};

};

#endif