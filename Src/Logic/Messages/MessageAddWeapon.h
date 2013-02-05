#ifndef __Logic_MessageAddWeapon_H
#define __Logic_MessageAddWeapon_H

#include "Message.h"

namespace Logic {

	class CMessageAddWeapon: public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddWeapon);
	public:
		CMessageAddWeapon();
		int getAddWeapon();
		void setAddWeapon( int weapon);
		~CMessageAddWeapon(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _weapon;
	};
	REG_FACTORYMESSAGE(CMessageAddWeapon);
};

#endif