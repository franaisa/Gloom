#ifndef __Logic_MessageAddWeapon_H
#define __Logic_MessageAddWeapon_H

#include "Message.h"

namespace Logic {

	class CMessageAddWeapon: public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddWeapon);
	public:
		CMessageAddWeapon();
		int getAddWeapon();
		int getAddAmmo();
		void setAddWeapon( int weapon);
		void setAddAmmo( int ammo);
		virtual ~CMessageAddWeapon(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _weapon;
		 int _ammo;
	};
	REG_FACTORYMESSAGE(CMessageAddWeapon);
};

#endif