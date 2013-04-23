#ifndef __Logic_MessageContactEnter_H
#define __Logic_MessageContactEnter_H

#include "Message.h"

namespace Logic {

	class CMessageContactEnter : public CMessage{
	DEC_FACTORYMESSAGE(CMessageContactEnter);
	public:
		CMessageContactEnter();
		Logic::TEntityID getEntity();
		void setEntity(Logic::TEntityID c);
		~CMessageContactEnter(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		Logic::TEntityID _entity;
	};
	REG_FACTORYMESSAGE(CMessageContactEnter);
};

#endif