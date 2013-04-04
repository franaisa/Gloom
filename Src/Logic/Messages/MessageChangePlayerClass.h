#ifndef __Logic_MessageChangePlayerClass_H
#define __Logic_MessageChangePlayerClass_H

#include "Message.h"

namespace Logic {

	class CMessageChangePlayerClass: public CMessage{
	DEC_FACTORYMESSAGE(CMessageChangePlayerClass);
	public:
		CMessageChangePlayerClass();
		virtual ~CMessageChangePlayerClass(){};
		
		void setPlayerClass(unsigned int classType);
		unsigned int getPlayerClass();

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		unsigned int _classType;
	};
	REG_FACTORYMESSAGE(CMessageChangePlayerClass);
};

#endif