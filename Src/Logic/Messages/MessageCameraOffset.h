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

		float getOffsetTimer() { return _fOffsetTimer; }
		void setOffsetTimer(float fOffsetTimer);
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		CEntity* _entity;

		/**
			Timer que se le asignará a la cámara con la vibración (offset) que tendrá
		*/
		float	_fOffsetTimer; 
	};
	REG_FACTORYMESSAGE(CMessageCameraOffset);
};

#endif