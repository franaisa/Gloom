#ifndef __Logic_MessageCameraOffsetEntity_H
#define __Logic_MessageCameraOffsetEntity_H

#include "Message.h"

namespace Logic {

	class CMessageCameraOffset: public CMessage{
	DEC_FACTORYMESSAGE(CMessageCameraOffset);
	public:
		CMessageCameraOffset();
		virtual ~CMessageCameraOffset();

		CEntity* getRelatedEntity();
		void setRelatedEntity(CEntity* entity);

		void setOffset(float fOffset);
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		CEntity* _entity;

		float	_fOffset;
	};
	REG_FACTORYMESSAGE(CMessageCameraOffset);
};

#endif