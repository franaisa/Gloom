#ifndef __Logic_MessageSecondaryShoot_H
#define __Logic_MessageSecondaryShoot_H

#include "Message.h"

namespace Logic {

	class CMessageSecondaryShoot : public CMessage{
	DEC_FACTORYMESSAGE(CMessageSecondaryShoot);
	public:
		CMessageSecondaryShoot(bool shoot);
		virtual ~CMessageSecondaryShoot(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setShoot(bool shoot){_shoot = shoot;}
		bool getShoot(){return _shoot;}

	private:
		bool _shoot;
	};
	REG_FACTORYMESSAGE(CMessageSecondaryShoot);
};

#endif