#ifndef __Logic_MessageCameraRollEntity_H
#define __Logic_MessageCameraRollEntity_H

#include "Message.h"

namespace Logic {

	class CMessageCameraRoll: public CMessage{
	DEC_FACTORYMESSAGE(CMessageCameraRoll);
	public:
		CMessageCameraRoll();
		virtual ~CMessageCameraRoll();

		CEntity* getRelatedEntity();

		float getRollDegrees			()					{ return _fRollDegrees; }
			void setRollDegrees			(float fDegrees);
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		CEntity* _entity;

		/**
			Grados que se rotará la cámara.
			Negativo = Hacia la izquierda ; Positivo = Hacia la derecha
		*/
		float	_fRollDegrees; 
	};
	REG_FACTORYMESSAGE(CMessageCameraRoll);
};

#endif