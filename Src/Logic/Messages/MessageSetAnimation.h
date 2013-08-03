#ifndef __Logic_MessageSetAnimation_H
#define __Logic_MessageSetAnimation_H

#include "Message.h"

namespace Logic {

	class CMessageSetAnimation : public CMessage{
	DEC_FACTORYMESSAGE(CMessageSetAnimation);
	public:
		CMessageSetAnimation();
		std::string getAnimation();
		void setAnimation(std::string);
		bool getLoop();
		void setLoop(bool loop);
		bool getExclude();
		void setExclude(bool exclude);
		void setRewind(int rewind);
		int getRewind();

		virtual ~CMessageSetAnimation(){};

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		std::string _animation;
		bool _loop;
		bool _exclude;
		int _rewind;
	};
	REG_FACTORYMESSAGE(CMessageSetAnimation);
};

#endif