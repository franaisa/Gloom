#ifndef __Logic_MessageBlockShoot_H
#define __Logic_MessageBlockShoot_H

#include "Message.h"

namespace Logic {

	class CMessageBlockShoot: public CMessage{
	DEC_FACTORYMESSAGE(CMessageBlockShoot);
	public:
		CMessageBlockShoot();
		virtual ~CMessageBlockShoot();

		bool canShoot();
		void canShoot(bool can);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		bool _canShoot;
	};
	REG_FACTORYMESSAGE(CMessageBlockShoot);
};

#endif