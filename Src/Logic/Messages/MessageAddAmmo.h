#ifndef __Logic_MessageAddAmmo_H
#define __Logic_MessageAddAmmo_H

#include "Message.h"

namespace Logic {

	class CMessageAddAmmo: public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddAmmo);
	public:
		CMessageAddAmmo();
		int getAddAmmo();
		void setAddAmmo( int ammo);
		~CMessageAddAmmo(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _ammo;
	};
	REG_FACTORYMESSAGE(CMessageAddAmmo);
};

#endif