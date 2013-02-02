#ifndef __Logic_MessageRebound_H
#define __Logic_MessageRebound_H

#include "Message.h"

namespace Logic {

	class CMessageRebound : public CMessage{
	public:
		CMessageRebound(TMessageType t);
		Vector3 getDir();
		void setDir(Vector3 dir);
		~CMessageRebound(){};
		
		virtual Net::CBuffer serialize();

	private:
		Vector3 _dir;
	};

};

#endif