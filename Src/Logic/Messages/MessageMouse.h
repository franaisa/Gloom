#ifndef __Logic_MessageMouse_H
#define __Logic_MessageMouse_H

#include "MessageControl.h"

namespace Logic {

	class CMessageMouse : public CMessageControl{
	public:
		CMessageMouse();
		void setMouse(float mouse[]);
		float* getMouse();
		~CMessageMouse(){};

		virtual Net::CBuffer* serialize();

	private:
		float _mouse[2];
	};

};

#endif