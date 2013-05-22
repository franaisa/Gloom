#ifndef __Logic_MessageSetRelatedEntity_H
#define __Logic_MessageSetRelatedEntity_H

#include "Message.h"

namespace Logic {

	class CMessageSetRelatedEntity: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSetRelatedEntity);
	public:
		CMessageSetRelatedEntity();
		virtual ~CMessageSetRelatedEntity();

		CEntity* getRelatedEntity();
		void setRelatedEntity(CEntity* entity);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		CEntity* _entity;
	};
	REG_FACTORYMESSAGE(CMessageSetRelatedEntity);
};

#endif