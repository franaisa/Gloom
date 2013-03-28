#ifndef __Logic_MessageSetImmunity_H
#define __Logic_MessageSetImmunity_H

#include "Message.h"

namespace Logic {

	class CMessageSetImmunity: public CMessage {
	DEC_FACTORYMESSAGE(CMessageSetImmunity);
	public:
		CMessageSetImmunity();
		~CMessageSetImmunity();

		bool isImmune();
		void setImmunity(bool isImmune);
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 bool _isImmune;
	};
	REG_FACTORYMESSAGE(CMessageSetImmunity);
};

#endif