#ifndef __Logic_MessageAddAmmo_H
#define __Logic_MessageAddAmmo_H

#include "Message.h"

namespace Logic {

	class CMessageAddAmmo: public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddAmmo);
	public:
		CMessageAddAmmo();
		virtual ~CMessageAddAmmo() {}

		int getAddAmmo();
		int getAddWeapon();
		void setAddAmmo( int ammo);
		void setAddWeapon( int weapon);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _weapon;
		 int _ammo;
	};
	REG_FACTORYMESSAGE(CMessageAddAmmo);
};

#endif