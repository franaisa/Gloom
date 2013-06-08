#ifndef __Logic_MessageTransform_H
#define __Logic_MessageTransform_H

#include "Message.h"

namespace Logic {

	class CMessageTransform : public CMessage{
	DEC_FACTORYMESSAGE(CMessageTransform);
	public:
		CMessageTransform();
		Vector3 getPosition();
		void setPosition(const Vector3& position);
		Quaternion getOrientation();
		void setOrientation(const Quaternion& position);
		void setMakeConversion(bool convertCoordsToLogicWorld);
		bool getMakeConversion();
		virtual ~CMessageTransform(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		Vector3 _position;
		Quaternion _orientation;
		bool _convertCoordsToLogicWorld;
	};
	REG_FACTORYMESSAGE(CMessageTransform);
};

#endif