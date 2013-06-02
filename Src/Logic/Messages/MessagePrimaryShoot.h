#ifndef __Logic_MessageActivate_H
#define __Logic_MessageActivate_H

#include "Message.h"

namespace Logic {

	class CMessageActivate : public CMessage{
	DEC_FACTORYMESSAGE(CMessageActivate);
	public:
		CMessageActivate();
		virtual ~CMessageActivate(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setActivated(bool activate){_activate = activate;}
		bool getActivated(){return _activate;}

	private:
		bool _activate;
	};
	REG_FACTORYMESSAGE(CMessageActivate);
};

#endif