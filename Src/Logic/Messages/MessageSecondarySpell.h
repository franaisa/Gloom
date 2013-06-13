#ifndef __Logic_MessageSecondarySpell_H
#define __Logic_MessageSecondarySpell_H

#include "Message.h"

namespace Logic {

	class CMessageSecondarySpell : public CMessage{
	DEC_FACTORYMESSAGE(CMessageSecondarySpell);
	public:
		CMessageSecondarySpell();
		CMessageSecondarySpell(bool spell);
		virtual ~CMessageSecondarySpell(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setSpell(bool spell){_spell = spell;}
		bool getSpell(){return _spell;}

	private:
		bool _spell;
	};
	REG_FACTORYMESSAGE(CMessageSecondarySpell);
};

#endif