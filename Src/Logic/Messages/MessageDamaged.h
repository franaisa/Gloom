#ifndef __Logic_MessageDamaged_H
#define __Logic_MessageDamaged_H

#include "Message.h"

namespace Logic {

	class CMessageDamaged: public CMessage{
	DEC_FACTORYMESSAGE(CMessageDamaged);
	public:
		CMessageDamaged();
		unsigned char getDamage();
		void setDamage(unsigned char damage);
		~CMessageDamaged(){};
		
		virtual Net::CBuffer* serialize();

	private:
		unsigned char _damage;
	};
	REG_FACTORYMESSAGE(CMessageDamaged);
};

#endif