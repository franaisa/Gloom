#ifndef __Logic_MessageContactEnter_H
#define __Logic_MessageContactEnter_H

#include "Message.h"
#include "Physics/ContactPoint.h"

namespace Logic {

	class CMessageContactEnter : public CMessage{
	DEC_FACTORYMESSAGE(CMessageContactEnter);
	public:
		CMessageContactEnter();
		~CMessageContactEnter(){};

		CEntity* getEntity();
		Physics::CContactPoint getContactPoint();

		void setEntity(CEntity* entity);
		void setContactPoint(const Physics::CContactPoint& contactPoint);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		CEntity* _entity;
		Physics::CContactPoint _contactPoint;
	};
	REG_FACTORYMESSAGE(CMessageContactEnter);
};

#endif