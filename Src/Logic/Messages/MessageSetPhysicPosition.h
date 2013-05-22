#ifndef __Logic_MessageSetPhysicPosition_H
#define __Logic_MessageSetPhysicPosition_H

#include "Message.h"

namespace Logic {

	class CMessageSetPhysicPosition: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSetPhysicPosition);
	public:
		CMessageSetPhysicPosition();
		Vector3 getPosition();
		void setPosition(const Vector3& direction);

		void setMakeConversion(bool convertCoordsToLogicWorld);
		bool getMakeConversion();

		virtual ~CMessageSetPhysicPosition(){};

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		Vector3 _position;
		bool _convertCoordsToLogicWorld;
	};
	REG_FACTORYMESSAGE(CMessageSetPhysicPosition);
};

#endif