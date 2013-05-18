#ifndef __Logic_MessageDamageAmplifier_H
#define __Logic_MessageDamageAmplifier_H

#include "Message.h"

namespace Logic {

	class CMessageDamageAmplifier: public CMessage{
	DEC_FACTORYMESSAGE(CMessageDamageAmplifier);
	public:
		CMessageDamageAmplifier();
		int getPercentDamage();
		void setPercentDamage(int percentDamage);
		~CMessageDamageAmplifier(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _percentDamage;
	};
	REG_FACTORYMESSAGE(CMessageDamageAmplifier);
};

#endif