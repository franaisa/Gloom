#ifndef __Logic_MessageDamageAmplifier_H
#define __Logic_MessageDamageAmplifier_H

#include "Message.h"

namespace Logic {

	class CMessageDamageAmplifier: public CMessage{
	DEC_FACTORYMESSAGE(CMessageDamageAmplifier);
	public:
		CMessageDamageAmplifier();
		CMessageDamageAmplifier(int percentDamage ) ;
		int getPercentDamage();
		void setPercentDamage(int percentDamage);
		void setDuration(unsigned int msecs) { _duration = msecs; }
		unsigned int getDuration() { return _duration; }
		~CMessageDamageAmplifier(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _percentDamage;
		 unsigned int _duration;
	};
	REG_FACTORYMESSAGE(CMessageDamageAmplifier);
};

#endif