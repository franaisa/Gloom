#ifndef __Logic_MessageSetReducedDamage_H
#define __Logic_MessageSetReducedDamage_H

#include "Message.h"

namespace Logic {

	class CMessageSetReducedDamage: public CMessage {
	DEC_FACTORYMESSAGE(CMessageSetReducedDamage);
	public:
		CMessageSetReducedDamage();
		virtual ~CMessageSetReducedDamage();

		float getReducedDamage();
		void setReducedDamage(float percentage);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 float _reducedDamage;
	};
	REG_FACTORYMESSAGE(CMessageSetReducedDamage);
};

#endif