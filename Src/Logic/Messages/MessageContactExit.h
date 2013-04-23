#ifndef __Logic_MessageContactExit_H
#define __Logic_MessageContactExit_H

#include "Message.h"

namespace Logic {

	class CMessageContactExit : public CMessage{
	DEC_FACTORYMESSAGE(CMessageContactExit);
	public:
		CMessageContactExit();
		Logic::TEntityID getEntity();
		void setEntity(Logic::TEntityID c);
		~CMessageContactExit(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		Logic::TEntityID _entity;
	};
	REG_FACTORYMESSAGE(CMessageContactExit);
};

#endif