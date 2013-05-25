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

	Net::CBuffer CMessageChangeMaterialHudWeapon::serialize() {
		// Tamaño de la cabecera (int) + nombre del material (string)
		Net::CBuffer buffer( sizeof(int) + (sizeof(char) * _materialName.size()) );
		buffer.serialize(std::string("CMessageChangeMaterialHudWeapon"), true);
		// Serializamos el string sin hacerle el crc
		buffer.serialize(_materialName, false);

		return buffer;
	}
	
	//__________________________________________________________________

	void CMessageChangeMaterialHudWeapon::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_materialName);
	}
};
