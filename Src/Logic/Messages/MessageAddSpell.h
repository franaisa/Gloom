#ifndef __Logic_MessageAddSpell_H
#define __Logic_MessageAddSpell_H

#include "Message.h"

namespace Logic {

	class CMessageAddSpell: public CMessage{
	DEC_FACTORYMESSAGE(CMessageAddSpell);
	public:
		CMessageAddSpell();
		int getSpell();
		void setSpell( int spell);
		virtual ~CMessageAddSpell(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 int _spell;
	};
	REG_FACTORYMESSAGE(CMessageAddSpell);
};

#endif