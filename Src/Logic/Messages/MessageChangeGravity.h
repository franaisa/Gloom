#ifndef __Logic_MessageChangeGravity_H
#define __Logic_MessageChangeGravity_H

#include "Message.h"

namespace Logic {

	class CMessageChangeGravity: public CMessage{
	DEC_FACTORYMESSAGE(CMessageChangeGravity);
	public:
		CMessageChangeGravity();
		CMessageChangeGravity(float gravity);
		virtual ~CMessageChangeGravity(){};

		float getGravity();
		void setGravity( float gravity);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 float _gravity;
	};
	REG_FACTORYMESSAGE(CMessageChangeGravity);
};

#endif