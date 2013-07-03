#ifndef __Logic_MessageSpellHungry_H
#define __Logic_MessageSpellHungry_H

#include "Message.h"

namespace Logic {

	class CMessageSpellHungry: public CMessage{
	DEC_FACTORYMESSAGE(CMessageSpellHungry);
	public:
		CMessageSpellHungry();
		CMessageSpellHungry(float gravity);
		virtual ~CMessageSpellHungry(){};

		float getSpellHungry();
		void setSpellHungry( float spellHungry);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 float _spellHungry;
	};
	REG_FACTORYMESSAGE(CMessageSpellHungry);
};

#endif