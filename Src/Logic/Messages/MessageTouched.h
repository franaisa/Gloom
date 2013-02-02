#ifndef __Logic_MessageTouched_H
#define __Logic_MessageTouched_H

#include "Message.h"

namespace Logic {

	class CMessageTouched : public CMessage{
	public:
		CMessageTouched(TMessageType t);
		CEntity* getEntity();
		void setEntity(CEntity *c);
		~CMessageTouched(){};
		
		virtual Net::CBuffer serialize();

	private:
		CEntity *_entity;
	};

};

#endif