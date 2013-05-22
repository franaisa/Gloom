#ifndef __Logic_MessageBerserker_H
#define __Logic_MessageBerserker_H

#include "Message.h"

namespace Logic {

	class CMessageBerserker: public CMessage{
	DEC_FACTORYMESSAGE(CMessageBerserker);
	public:
		CMessageBerserker();
		int getPercentDamage();
		void setPercentDamage(int percentDamage);
		int getPercentCooldown();
		void setPercentCooldown(int percentCooldown);
		virtual ~CMessageBerserker(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _percentDamage;
		 int _percentCooldown;
	};
	REG_FACTORYMESSAGE(CMessageBerserker);
};

#endif