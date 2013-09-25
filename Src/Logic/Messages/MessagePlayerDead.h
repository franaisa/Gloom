#ifndef __Logic_MessagePlayerDead_H
#define __Logic_MessagePlayerDead_H

#include "Message.h"

namespace Logic {

	class CEntity;

	class CMessagePlayerDead: public CMessage{
	DEC_FACTORYMESSAGE(CMessagePlayerDead);
	public:
		CMessagePlayerDead();
		virtual ~CMessagePlayerDead(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setKiller(CEntity* killer) { _killer = killer; }
		CEntity* getKiller() { return _killer; }

	private:
		CEntity* _killer;
	};
	REG_FACTORYMESSAGE(CMessagePlayerDead);
};

#endif