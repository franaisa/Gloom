#ifndef __Logic_MessageChangeMaterialHudWeapon_H
#define __Logic_MessageChangeMaterialHudWeapon_H

#include "Message.h"

#include <string>

namespace Logic {

	class CMessageChangeMaterialHudWeapon: public CMessage{
	DEC_FACTORYMESSAGE(CMessageChangeMaterialHudWeapon);
	public:
		CMessageChangeMaterialHudWeapon();
		virtual ~CMessageChangeMaterialHudWeapon(){};
		
		void setMaterialName(const std::string& materialName);
		std::string getMaterialName();

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		std::string _materialName;
	};
	REG_FACTORYMESSAGE(CMessageChangeMaterialHudWeapon);
};

#endif