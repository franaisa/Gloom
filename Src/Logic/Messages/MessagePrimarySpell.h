#ifndef __Logic_MessagePrimarySpell_H
#define __Logic_MessagePrimarySpell_H

#include "Message.h"

namespace Logic {

	class CMessagePrimarySpell : public CMessage{
	DEC_FACTORYMESSAGE(CMessagePrimarySpell);
	public:
		CMessagePrimarySpell();
		CMessagePrimarySpell(bool spell);
		virtual ~CMessagePrimarySpell(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setSpell(bool spell){_spell = spell;}
		bool getSpell(){return _spell;}

	private:
		bool _spell;
	};
	REG_FACTORYMESSAGE(CMessagePrimarySpell);
};

#endif