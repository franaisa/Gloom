#ifndef __Logic_MessageDamaged_H
#define __Logic_MessageDamaged_H

#include "Message.h"

namespace Logic {

	class CMessageDamaged: public CMessage{
	DEC_FACTORYMESSAGE(CMessageDamaged);
	public:
		CMessageDamaged();
		float getDamage();
		void setDamage(float damage);
		CEntity* getEnemy();
		void setEnemy(CEntity* entity);
		~CMessageDamaged(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		float _damage;
		CEntity* _entity;
	};
	REG_FACTORYMESSAGE(CMessageDamaged);
};

#endif