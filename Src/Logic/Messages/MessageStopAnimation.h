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
		bool getBool();
		void setBool(bool boolean);
		~CMessageStopAnimation(){};

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		std::string _string;
		bool _bool;
	};
	REG_FACTORYMESSAGE(CMessageStopAnimation);
};

#endif