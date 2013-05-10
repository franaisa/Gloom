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
		void setSeqNumber(unsigned int seq);
		unsigned int getSeqNumber();
		~CMessageControl(){};

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	protected:
		ControlType _controlType;
		unsigned int _seq;
	};
	REG_FACTORYMESSAGE(CMessageControl);
};

#endif