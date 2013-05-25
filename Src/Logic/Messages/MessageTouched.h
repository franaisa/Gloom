#ifndef __Logic_MessageTouched_H
#define __Logic_MessageTouched_H

#include "Message.h"

namespace Logic {

	class CMessageTouched : public CMessage{
	DEC_FACTORYMESSAGE(CMessageTouched);
	public:
		CMessageTouched();
		CEntity* getEntity();
		void setEntity(CEntity *c);
		virtual ~CMessageTouched(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		CEntity *_entity;
	};
	REG_FACTORYMESSAGE(CMessageTouched);
};

#endif