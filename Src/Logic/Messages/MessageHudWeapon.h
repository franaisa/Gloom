#ifndef __Logic_MessageHudWeapon_H
#define __Logic_MessageHudWeapon_H

#include "Message.h"

namespace Logic {

	class CMessageHudWeapon: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudWeapon);
	public:
		CMessageHudWeapon();
		int getWeapon();
		void setWeapon( int weapon);
		~CMessageHudWeapon(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _weapon;
	};
	REG_FACTORYMESSAGE(CMessageHudWeapon);
};

#endif