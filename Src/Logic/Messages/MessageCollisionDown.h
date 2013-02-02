#ifndef __Logic_MessageCollisionDown_H
#define __Logic_MessageCollisionDown_H

#include "Message.h"

namespace Logic {

	class CMessageCollisionDown : public CMessage{
	DEC_FACTORYMESSAGE(CMessageCollisionDown);
	public:
		CMessageCollisionDown();
		bool getCollisionDown();
		void setCollisionDown(bool collision);
		~CMessageCollisionDown(){};
		
		virtual Net::CBuffer* serialize();
	private:
		bool _collision;
	};
	REG_FACTORYMESSAGE(CMessageCollisionDown);
};

#endif