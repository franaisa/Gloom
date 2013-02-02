#ifndef __Logic_MessageControl_H
#define __Logic_MessageControl_H

#include "Message.h"

namespace Logic {

	class CMessageControl : public CMessage{
	DEC_FACTORYMESSAGE(CMessageControl);
	public:
		CMessageControl();
		ControlType getType();
		void setType(ControlType controltype);
		~CMessageControl(){};

		virtual Net::CBuffer* serialize();

	protected:
		ControlType _controlType;
	};
	REG_FACTORYMESSAGE(CMessageControl);
};

#endif