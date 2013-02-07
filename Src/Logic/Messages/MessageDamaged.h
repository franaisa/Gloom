#ifndef __Logic_MessageDamaged_H
#define __Logic_MessageDamaged_H

#include "Message.h"

namespace Logic {

	class CMessageDamaged: public CMessage{
	DEC_FACTORYMESSAGE(CMessageDamaged);
	public:
		CMessageDamaged();
		unsigned char getDamage();
		void setDamage(unsigned char damage);
		CEntity* getEnemy();
		void setEnemy(CEntity* entity);
		~CMessageDamaged(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		unsigned char _damage;
		CEntity* _entity;
	};
	REG_FACTORYMESSAGE(CMessageDamaged);
};

#endif