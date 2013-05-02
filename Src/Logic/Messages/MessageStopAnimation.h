#ifndef __Logic_MessageStopAnimation_H
#define __Logic_MessageStopAnimation_H

#include "Message.h"

namespace Logic {

	class CMessageStopAnimation : public CMessage{
	DEC_FACTORYMESSAGE(CMessageStopAnimation);
	public:
		CMessageStopAnimation();
		std::string getString();
		void setString(std::string);
		~CMessageStopAnimation(){};

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		std::string _string;
	};
	REG_FACTORYMESSAGE(CMessageStopAnimation);
};

#endif