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

		Vector3			getPosition		()	{return _vPosition; }
		void				setPosition		(Vector3 vValue) { _vPosition = vValue; }

		std::string 	getTexture		()	{return _vTexture; }
		void				setTexture		(std::string vValue) { _vTexture = vValue; }

	private:
		Vector3		_vPosition;
		std::string	_vTexture;
		
	};
	REG_FACTORYMESSAGE(CMessageDecal);
};

#endif