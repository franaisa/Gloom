#ifndef __Logic_MessageTransformSnapshot_H
#define __Logic_MessageTransformSnapshot_H

#include "Message.h"

namespace Logic {

	class CMessageTransformSnapshot: public CMessage {
	DEC_FACTORYMESSAGE(CMessageTransformSnapshot);
	public:
		CMessageTransformSnapshot();
		virtual ~CMessageTransformSnapshot();

		void setBuffer(const std::vector<Matrix4> buffer);
		std::vector<Matrix4> getBuffer();
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 std::vector<Matrix4> _transformBuffer;
	};
	REG_FACTORYMESSAGE(CMessageTransformSnapshot);
};

#endif