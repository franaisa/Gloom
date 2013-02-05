#ifndef __Logic_MessageSetPhysicPosition_H
#define __Logic_MessageSetPhysicPosition_H

#include "Message.h"

namespace Logic {

	class CMessageSetPhysicPosition: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSetPhysicPosition);
	public:
		CMessageSetPhysicPosition();
		Vector3 getPosition();
		void setPosition(Vector3 direction);
		~CMessageSetPhysicPosition(){};

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		Vector3 _position;
	};
	REG_FACTORYMESSAGE(CMessageSetPhysicPosition);
};

#endif