#ifndef __Logic_MessageMouse_H
#define __Logic_MessageMouse_H

#include "MessageControl.h"

namespace Logic {

	class CMessageMouse : public CMessageControl{
	DEC_FACTORYMESSAGE(CMessageMouse);
	public:
		CMessageMouse();
		void setMouse(float mouse[]);
		float* getMouse();
		~CMessageMouse(){};

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		float _mouse[2];
	};
	REG_FACTORYMESSAGE(CMessageMouse);
};

#endif