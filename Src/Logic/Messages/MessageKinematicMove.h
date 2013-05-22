#ifndef __Logic_MessageKinematicMove_H
#define __Logic_MessageKinematicMove_H

#include "Message.h"

namespace Logic {

	class CMessageKinematicMove: public CMessage{
	DEC_FACTORYMESSAGE(CMessageKinematicMove);
	public:
		CMessageKinematicMove();
		Vector3 getMovement();
		void setMovement(Vector3 movement);
		virtual ~CMessageKinematicMove(){};

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		Vector3 _movement;
	};
	REG_FACTORYMESSAGE(CMessageKinematicMove);
};

#endif