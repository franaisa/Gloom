#ifndef __Logic_MessageAddForcePlayer_H
#define __Logic_MessageAddForcePlayer_H

#include "Message.h"

namespace Logic {

	class CMessageAddForcePlayer : public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddForcePlayer);
	public:
		CMessageAddForcePlayer();
		void setPower(float power);
		float getPower();
		void setVelocity(float velocity);
		float getVelocity();
		void setDirection(Vector3 direction);
		Vector3 getDirection();
		~CMessageAddForcePlayer(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	protected:
		float _power;
		float _velocity;
		Vector3 _direction;
	};
	REG_FACTORYMESSAGE(CMessageAddForcePlayer);

};

#endif