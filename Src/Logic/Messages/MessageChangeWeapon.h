#ifndef __Logic_MessageChangeWeapon_H
#define __Logic_MessageChangeWeapon_H

#include "Message.h"

namespace Logic {

	class CMessageChangeWeapon: public CMessage{
	DEC_FACTORYMESSAGE(CMessageChangeWeapon);
	public:
		CMessageChangeWeapon();
		int getWeapon();
		void setWeapon(int weapon);
		~CMessageChangeWeapon() { };

		virtual Net::CBuffer* serialize();
	private:
		int _weapon;
	};
	REG_FACTORYMESSAGE(CMessageChangeWeapon);
};

#endif