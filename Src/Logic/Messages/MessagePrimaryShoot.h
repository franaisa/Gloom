#ifndef __Logic_MessagePrimaryShoot_H
#define __Logic_MessagePrimaryShoot_H

#include "Message.h"

namespace Logic {

	class CMessagePrimaryShoot : public CMessage{
	DEC_FACTORYMESSAGE(CMessagePrimaryShoot);
	public:
		CMessagePrimaryShoot(bool shoot);
		virtual ~CMessagePrimaryShoot(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setShoot(bool shoot){_shoot = shoot;}
		bool getShoot(){return _shoot;}

	private:
		bool _shoot;
	};
	REG_FACTORYMESSAGE(CMessagePrimaryShoot);
};

#endif