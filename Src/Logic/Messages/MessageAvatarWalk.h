#ifndef __Logic_MessageAvatarWalk_H
#define __Logic_MessageAvatarWalk_H

#include "Message.h"

namespace Logic {

	class CMessageAvatarWalk: public CMessage{
	public:
		CMessageAvatarWalk(TMessageType t);
		Vector3 getDirection();
		void setDirection(Vector3 direction);
		~CMessageAvatarWalk(){};

		virtual Net::CBuffer serialize();
		
	private:
		Vector3 _direction;
	};

};

#endif