#ifndef __Logic_MessageHudDispersion_H
#define __Logic_MessageHudDispersion_H

#include "Message.h"

namespace Logic {

	class CMessageHudDispersion : public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudDispersion);
	public:
		CMessageHudDispersion();
		~CMessageHudDispersion(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		float	getHeight		()					{ return _fHeight;}
		void		setHeight		(float fValue)		{ _fHeight = fValue; }

		float	getWidth		()					{ return _fWidth;}
		void		setWidth		(float fValue)		{ _fWidth = fValue; }

	private:
		float	_fHeight;
		float	_fWidth;

	};
	REG_FACTORYMESSAGE(CMessageHudDispersion);
};

#endif