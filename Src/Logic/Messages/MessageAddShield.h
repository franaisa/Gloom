#ifndef __Logic_MessageAddShield_H
#define __Logic_MessageAddShield_H

#include "Message.h"

namespace Logic {

	class CMessageAddShield: public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddShield);
	public:
		CMessageAddShield();
		int getAddShield();
		void setAddShield( int shield);
		~CMessageAddShield(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _shield;
	};
	REG_FACTORYMESSAGE(CMessageAddShield);
};

#endif