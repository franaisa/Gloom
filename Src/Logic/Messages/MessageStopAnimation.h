#ifndef __Logic_MessageStopAnimation_H
#define __Logic_MessageStopAnimation_H

#include "Message.h"

namespace Logic {

	class CMessageStopAnimation : public CMessage{
	public:
		CMessageStopAnimation(TMessageType t);
		std::string getString();
		void setString(std::string);
		bool getBool();
		void setBool(bool boolean);
		~CMessageStopAnimation(){};

		virtual Net::CBuffer* serialize();
		
	private:
		std::string _string;
		bool _bool;
	};

};

#endif