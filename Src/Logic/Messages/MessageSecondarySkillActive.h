#ifndef __Logic_MessageSecondarySkillActive_H
#define __Logic_MessageSecondarySkillActive_H

#include "Message.h"

namespace Logic {

	class CMessageSecondarySkillActive : public CMessage{
	DEC_FACTORYMESSAGE(CMessageSecondarySkillActive);
	public:
		CMessageSecondarySkillActive();
		virtual ~CMessageSecondarySkillActive(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		bool canActive();
		void canActive(bool active);

	private:
		bool _active;
	};
	REG_FACTORYMESSAGE(CMessageSecondarySkillActive);
};

#endif