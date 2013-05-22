#ifndef __Logic_MessageChangeMaterial_H
#define __Logic_MessageChangeMaterial_H

#include "Message.h"

#include <string>

namespace Logic {

	class CMessageChangeMaterial: public CMessage{
	DEC_FACTORYMESSAGE(CMessageChangeMaterial);
	public:
		CMessageChangeMaterial();
		virtual ~CMessageChangeMaterial(){};
		
		void setMaterialName(const std::string& materialName);
		std::string getMaterialName();

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		std::string _materialName;
	};
	REG_FACTORYMESSAGE(CMessageChangeMaterial);
};

#endif