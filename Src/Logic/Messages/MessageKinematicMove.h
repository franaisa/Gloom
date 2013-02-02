#ifndef __Logic_MessageKinematicMove_H
#define __Logic_MessageKinematicMove_H

#include "Message.h"

namespace Logic {

	class CMessageKinematicMove: public CMessage{
	public:
		CMessageKinematicMove(TMessageType t);
		Vector3 getMovement();
		void setMovement(Vector3 movement);
		~CMessageKinematicMove(){};

		virtual Net::CBuffer serialize();
		
	private:
		Vector3 _movement;
	};

};

#endif