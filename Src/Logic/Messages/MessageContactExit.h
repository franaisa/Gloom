#ifndef __Logic_MessageContactExit_H
#define __Logic_MessageContactExit_H

#include "Message.h"

namespace Logic {

	class CMessageContactExit : public CMessage{
	DEC_FACTORYMESSAGE(CMessageContactExit);
	public:
		CMessageContactExit();
		CEntity* getEntity();
		void setEntity(CEntity *c);
		~CMessageContactExit(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		CEntity *_entity;
	};
	REG_FACTORYMESSAGE(CMessageContactExit);
};

#endif