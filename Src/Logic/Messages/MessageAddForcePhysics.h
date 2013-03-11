#ifndef __Logic_MessageAddForcePhysics_H
#define __Logic_MessageAddForcePhysics_H

#include "Message.h"

namespace Logic {

	class CMessageAddForcePhysics : public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddForcePhysics);
	public:
		CMessageAddForcePhysics();

		void setForceVector(const Vector3& force);
		Vector3 getForceVector();
		void setGravity(bool gravity);
		bool getGravity();
		
		~CMessageAddForcePhysics(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	protected:
		Vector3 _force;
		bool _gravity;
	};
	REG_FACTORYMESSAGE(CMessageAddForcePhysics);

};

#endif