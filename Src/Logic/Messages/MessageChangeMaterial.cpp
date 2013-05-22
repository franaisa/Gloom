#include "MessageChangeMaterial.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageChangeMaterial);

	//__________________________________________________________________
	
	CMessageChangeMaterial::CMessageChangeMaterial() : CMessage(Message::CHANGE_MATERIAL) {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessageChangeMaterial::setMaterialName(const std::string& materialName) {
		this->_materialName = materialName;
	}
	
	//__________________________________________________________________

	std::string CMessageChangeMaterial::getMaterialName() {
		return _materialName;
	}
	
	//__________________________________________________________________

	Net::CBuffer CMessageChangeMaterial::serialize() {
		// Tamaño de la cabecera (int) + nombre del material (string)
		Net::CBuffer buffer( sizeof(int) + (sizeof(char) * _materialName.size()) );
		buffer.serialize(std::string("CMessageChangeMaterial"), true);
		// Serializamos el string sin hacerle el crc
		buffer.serialize(_materialName, false);

		return buffer;
	}
	
	//__________________________________________________________________

	void CMessageChangeMaterial::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_materialName);
	}
};
