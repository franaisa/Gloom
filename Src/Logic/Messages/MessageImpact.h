#ifndef __Logic_MessageImpact_H
#define __Logic_MessageImpact_H

#include "Message.h"

namespace Logic {

	class CMessageImpact: public CMessage{
	DEC_FACTORYMESSAGE(CMessageImpact);
	public:
		CMessageImpact();
		virtual ~CMessageImpact();

		CEntity* getRelatedEntity();

		float getDirectionImpact		() { return _fAngleImpactDirection; }
		void	setDirectionImpact		(float fAngle);
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		CEntity* _entity;

		/**
			Angulo con la dirección del impacto
		*/
		float	_fAngleImpactDirection; 
	};
	REG_FACTORYMESSAGE(CMessageImpact);
};

#endif