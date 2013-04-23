#include "MessageChangeMaterialHudWeapon.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageChangeMaterialHudWeapon);

	//__________________________________________________________________
	
	CMessageChangeMaterialHudWeapon::CMessageChangeMaterialHudWeapon() : CMessage(Message::CHANGE_MATERIAL_HUD_WEAPON) {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessageChangeMaterialHudWeapon::setMaterialName(const std::string& materialName) {
		this->_materialName = materialName;
	}
	
	//__________________________________________________________________

	std::string CMessageChangeMaterialHudWeapon::getMaterialName() {
		return _materialName;
	}
	
	//__________________________________________________________________

	Net::CBuffer* CMessageChangeMaterialHudWeapon::serialize() {
		assert(_tempBuffer == NULL);

		// Tamaño de la cabecera (int) + nombre del material (string)
		_tempBuffer = new Net::CBuffer( sizeof(int) + (sizeof(char) * _materialName.size()) );
		_tempBuffer->serialize(std::string("CMessageChangeMaterialHudWeapon"), true);
		// Serializamos el string sin hacerle el crc
		_tempBuffer->serialize(_materialName, false);

		return _tempBuffer;
	}
	
	//__________________________________________________________________

	void CMessageChangeMaterialHudWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_materialName);
	}
};
