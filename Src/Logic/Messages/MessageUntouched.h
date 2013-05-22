#ifndef __Logic_MessageUntouched_H
#define __Logic_MessageUntouched_H

#include "Message.h"

namespace Logic {

	class CMessageUntouched : public CMessage{
	DEC_FACTORYMESSAGE(CMessageUntouched);
	public:
		CMessageUntouched();
		CEntity* getEntity();
		void setEntity(CEntity *c);
		virtual ~CMessageUntouched(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		CEntity *_entity;
	};
	REG_FACTORYMESSAGE(CMessageUntouched);
};

#endif