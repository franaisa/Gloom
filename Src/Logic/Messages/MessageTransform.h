#ifndef __Logic_MessageTransform_H
#define __Logic_MessageTransform_H

#include "Message.h"

namespace Logic {

	class CMessageTransform : public CMessage{
	public:
		CMessageTransform(TMessageType t);
		Matrix4 getTransform();
		void setTransform(Matrix4 transform);
		~CMessageTransform(){};
		
		virtual Net::CBuffer serialize();

	private:
		Matrix4 _transform;
	};

};

#endif