#ifndef __Logic_MessageAddForcePhysics_H
#define __Logic_MessageAddForcePhysics_H

#include "Message.h"
#include "Physics/DynamicEntity.h"

namespace Logic {

	class CMessageAddForcePhysics : public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddForcePhysics);
	public:
		CMessageAddForcePhysics();
		virtual ~CMessageAddForcePhysics(){};

		void setForce(const Vector3& force, Physics::ForceMode mode);

		Physics::ForceMode getForceMode();
		Vector3 getForceVector();

		void setGravity(bool gravity);
		bool getGravity();
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	protected:
		Vector3 _force;
		Physics::ForceMode _forceMode;
		bool _gravity;
	};
	REG_FACTORYMESSAGE(CMessageAddForcePhysics);

};

#endif