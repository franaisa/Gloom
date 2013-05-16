#ifndef __Logic_MessageFlash_H
#define __Logic_MessageFlash_H

#include "Message.h"

namespace Logic {

	class CMessageFlash : public CMessage{
	DEC_FACTORYMESSAGE(CMessageFlash);
	public:
		CMessageFlash();
		~CMessageFlash(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		void setFlashFactor(const float &flashFactor);
		float getFlashFactor();

	private:
		float _flashFactor;
	};
	REG_FACTORYMESSAGE(CMessageFlash);

};

#endif