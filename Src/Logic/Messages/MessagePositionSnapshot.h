#ifndef __Logic_MessagePositionSnapshot_H
#define __Logic_MessagePositionSnapshot_H

#include "Message.h"

namespace Logic {

	class CMessagePositionSnapshot: public CMessage {
	DEC_FACTORYMESSAGE(CMessagePositionSnapshot);
	public:
		CMessagePositionSnapshot();
		virtual ~CMessagePositionSnapshot();

		void setBuffer(const std::vector<Vector3> buffer);
		std::vector<Vector3> getBuffer();
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 std::vector<Vector3> _positionBuffer;
	};
	REG_FACTORYMESSAGE(CMessagePositionSnapshot);
};

#endif