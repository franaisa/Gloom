#ifndef __Logic_MessageChangeWeaponGraphics_H
#define __Logic_MessageChangeWeaponGraphics_H

#include "Message.h"

namespace Logic {

	class CMessageChangeWeaponGraphics: public CMessage{
	public:
		CMessageChangeWeaponGraphics(TMessageType t);
		int getWeapon();
		void setWeapon(int weapon);
		~CMessageChangeWeaponGraphics() { };

		virtual Net::CBuffer* serialize();
	private:
		int _weapon;
	};

};

#endif