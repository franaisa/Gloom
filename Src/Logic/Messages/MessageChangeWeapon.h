#ifndef __Logic_MessageChangeWeapon_H
#define __Logic_MessageChangeWeapon_H

#include "Message.h"

namespace Logic {

	class CMessageChangeWeapon: public CMessage{
	DEC_FACTORYMESSAGE(CMessageChangeWeapon);
	public:
		CMessageChangeWeapon();
		virtual ~CMessageChangeWeapon() { };

		int		getWeapon		();
		
		/**
		Se le pasa por parámetro el índice en el array de armas al que se cambiará de arma
		También puede tener de parámetro 100 (scroll->siguiente arma) ó -100 (scroll->arma anterior)
		Lo he hecho de esta manera para no tener que crear un mensaje nuevo, y adaptar éste ya existente
		*/
		void	setWeapon		(int weapon); 


		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		int _weapon;
	};
	REG_FACTORYMESSAGE(CMessageChangeWeapon);
};

#endif