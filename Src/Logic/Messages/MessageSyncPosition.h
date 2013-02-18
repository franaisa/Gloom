#ifndef __Logic_MessageSyncPosition_H
#define __Logic_MessageSyncPosition_H

#include "Message.h"

namespace Logic {

	class CMessageSyncPosition: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSyncPosition);
	public:
		CMessageSyncPosition();
		Matrix4 getTransform();
		void setTransform(const Matrix4& transform);
		~CMessageSyncPosition(){};

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		Matrix4 _transform;
	};
	REG_FACTORYMESSAGE(CMessageSyncPosition);
};

#endif