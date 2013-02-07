#ifndef __Logic_MessageCameraToEnemy_H
#define __Logic_MessageCameraToEnemy_H

#include "Message.h"

namespace Logic {

	class CMessageCameraToEnemy: public CMessage{
	DEC_FACTORYMESSAGE(CMessageCameraToEnemy);
	public:
		CMessageCameraToEnemy();
		CEntity* getEnemy();
		void setEnemy(CEntity* entity);
		~CMessageCameraToEnemy(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		CEntity* _entity;
	};
	REG_FACTORYMESSAGE(CMessageCameraToEnemy);
};

#endif