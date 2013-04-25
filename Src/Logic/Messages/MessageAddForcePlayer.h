#ifndef __Logic_MessageAddForcePlayer_H
#define __Logic_MessageAddForcePlayer_H

#include "Message.h"

namespace Logic {

	class CMessageAddForcePlayer : public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddForcePlayer);
	public:
		CMessageAddForcePlayer();
		void setForce(Vector3 force);
		Vector3 getForce();
		~CMessageAddForcePlayer(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	protected:
		Vector3 _force;
	};
	REG_FACTORYMESSAGE(CMessageAddForcePlayer);

};

#endif