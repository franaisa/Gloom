#ifndef __Logic_MessageUntouched_H
#define __Logic_MessageUntouched_H

#include "Message.h"

namespace Logic {

	class CMessageUntouched : public CMessage{
	public:
		CMessageUntouched(TMessageType t);
		CEntity* getEntity();
		void setEntity(CEntity *c);
		~CMessageUntouched(){};
		
		virtual Net::CBuffer* serialize();

	private:
		CEntity *_entity;
	};

};

#endif