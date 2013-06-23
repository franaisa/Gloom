#ifndef __Logic_MessageHud_H
#define __Logic_MessageHud_H

#include "Message.h"

namespace Logic {

	class CMessageHud: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHud);

	enum HudType{
		PRIMARY_SKILL,
		SECONDARY_SKILL,
		HIT
	};

	public:
		CMessageHud();
		HudType getType();
		void setType( HudType type);
		virtual ~CMessageHud(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		HudType _type;
	};
	REG_FACTORYMESSAGE(CMessageHud);
};

#endif