#ifndef __Logic_MessageControl_H
#define __Logic_MessageControl_H

#include "Message.h"

namespace Logic {

	class CMessageControl : public CMessage{
	public:
		CMessageControl(TMessageType t);
		ControlType getType();
		void setType(ControlType controltype);
		~CMessageControl(){};

		virtual Net::CBuffer serialize();

	protected:
		ControlType _controlType;
	};

};

#endif