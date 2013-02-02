#ifndef __Logic_MessageChangeWeapon_H
#define __Logic_MessageChangeWeapon_H

#include "Message.h"

namespace Logic {

	class CMessageChangeWeapon: public CMessage{
	public:
		CMessageChangeWeapon(TMessageType t);
		int getWeapon();
		void setWeapon(int weapon);
		~CMessageChangeWeapon() { };

		virtual Net::CBuffer serialize();
	private:
		int _weapon;
	};

};

#endif