#ifndef __Logic_MessageSetOwner_H
#define __Logic_MessageSetOwner_H

#include "Message.h"

namespace Logic {

	class CMessageSetOwner: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSetOwner);
	public:
		CMessageSetOwner();
		virtual ~CMessageSetOwner();

		CEntity* getOwner();
		void setOwner(CEntity* entity);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		CEntity* _entity;
	};
	REG_FACTORYMESSAGE(CMessageSetOwner);
};

#endif