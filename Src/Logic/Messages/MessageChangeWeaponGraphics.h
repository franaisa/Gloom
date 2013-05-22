#ifndef __Logic_MessageChangeWeaponGraphics_H
#define __Logic_MessageChangeWeaponGraphics_H

#include "Message.h"

namespace Logic {

	class CMessageChangeWeaponGraphics: public CMessage{
	DEC_FACTORYMESSAGE(CMessageChangeWeaponGraphics);
	public:
		CMessageChangeWeaponGraphics();
		int getWeapon();
		void setWeapon(int weapon);
		~CMessageChangeWeaponGraphics() { };

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		int _weapon;
	};
	REG_FACTORYMESSAGE(CMessageChangeWeaponGraphics);
};

#endif