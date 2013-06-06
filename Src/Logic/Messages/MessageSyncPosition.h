#ifndef __Logic_MessageSyncPosition_H
#define __Logic_MessageSyncPosition_H

#include "Message.h"

namespace Logic {

	class CMessageSyncPosition: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSyncPosition);
	public:
		CMessageSyncPosition();
		Vector3 getPosition();
		void setPosition(const Vector3& position);
		Quaternion getOrientation();
		void setOrientation(const Quaternion& orientation);

		virtual ~CMessageSyncPosition(){};

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		Vector3 _position;
		Quaternion _orientation;
	};
	REG_FACTORYMESSAGE(CMessageSyncPosition);
};

#endif