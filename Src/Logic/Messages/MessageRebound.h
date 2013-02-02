#ifndef __Logic_MessageRebound_H
#define __Logic_MessageRebound_H

#include "Message.h"

namespace Logic {

	class CMessageRebound : public CMessage{
	public:
		CMessageRebound();
		Vector3 getDir();
		void setDir(Vector3 dir);
		~CMessageRebound(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		Vector3 _dir;
	};

};

#endif