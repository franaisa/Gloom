#ifndef __Logic_MessageDecal_H
#define __Logic_MessageDecal_H

#include "Message.h"

namespace Logic {

	class CMessageDecal : public CMessage{
	DEC_FACTORYMESSAGE(CMessageDecal);
	public:
		CMessageDecal();
		virtual ~CMessageDecal(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		Vector3		getPosition		()	{return _vPosition; }
		void			setPosition		(Vector3 vValue) { _vPosition = vValue; }

	private:
		Vector3		_vPosition;
		
	};
	REG_FACTORYMESSAGE(CMessageDecal);
};

#endif