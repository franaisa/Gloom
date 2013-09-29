#ifndef __Logic_MessageParticleStop_H
#define __Logic_MessageParticleStop_H

#include "Message.h"

namespace Logic {

	class CMessageParticleStop : public CMessage{
	DEC_FACTORYMESSAGE(CMessageParticleStop);
	public:
		CMessageParticleStop();
		virtual ~CMessageParticleStop();
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	};
	REG_FACTORYMESSAGE(CMessageParticleStop);

};

#endif