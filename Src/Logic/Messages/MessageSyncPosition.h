#ifndef __Logic_MessageSyncPosition_H
#define __Logic_MessageSyncPosition_H

#include "Message.h"

namespace Logic {

	class CMessageSyncPosition: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSyncPosition);
	public:
		CMessageSyncPosition();
		Vector3 getPosition();
		unsigned int getSeq();
		void setPosition(const Vector3& position);
		void setSeq(const unsigned int seq);
		virtual ~CMessageSyncPosition(){};

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		Vector3 _position;
		unsigned int _seq;
	};
	REG_FACTORYMESSAGE(CMessageSyncPosition);
};

#endif