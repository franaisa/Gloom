#ifndef __Logic_MessageHudAmmo_H
#define __Logic_MessageHudAmmo_H

#include "Message.h"

namespace Logic {

	class CMessageHudAmmo: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudAmmo);
	public:
		CMessageHudAmmo();
		int getAmmo();
		void setAmmo( int ammo);
		int getWeapon();
		void setWeapon( int weapon);
		~CMessageHudAmmo(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _ammo;
		 int _weapon;
	};
	REG_FACTORYMESSAGE(CMessageHudAmmo);
};

#endif