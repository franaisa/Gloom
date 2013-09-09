#ifndef __Logic_MessageParticleStart_H
#define __Logic_MessageParticleStart_H

#include "Message.h"

namespace Logic {

	class CMessageParticleStart : public CMessage{
	DEC_FACTORYMESSAGE(CMessageParticleStart);
	public:
		CMessageParticleStart();
		virtual ~CMessageParticleStart();
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	};
	REG_FACTORYMESSAGE(CMessageParticleStart);

};

#endif