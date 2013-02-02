#ifndef __Logic_MessageAvatarWalk_H
#define __Logic_MessageAvatarWalk_H

#include "Message.h"

namespace Logic {

	class CMessageAvatarWalk: public CMessage{
	DEC_FACTORYMESSAGE(CMessageAvatarWalk);
	public:
		CMessageAvatarWalk();
		Vector3 getDirection();
		void setDirection(Vector3 direction);
		~CMessageAvatarWalk(){};

		virtual Net::CBuffer* serialize();
		
	private:
		Vector3 _direction;
	};
	REG_FACTORYMESSAGE(CMessageAvatarWalk);
};

#endif