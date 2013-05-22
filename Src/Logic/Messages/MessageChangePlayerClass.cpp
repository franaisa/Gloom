#include "MessageChangePlayerClass.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageChangePlayerClass);

	//__________________________________________________________________
	
	CMessageChangePlayerClass::CMessageChangePlayerClass() : CMessage(Message::CHANGE_PLAYER_CLASS) {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessageChangePlayerClass::setPlayerClass(unsigned int classType) {
		this->_classType = classType;
	}
	
	//__________________________________________________________________

	unsigned int CMessageChangePlayerClass::getPlayerClass() {
		return _classType;
	}
	
	//__________________________________________________________________

	Net::CBuffer CMessageChangePlayerClass::serialize() {
		// Tamaño de la cabecera (int) + el tipo de la clase (unsigned int)
		Net::CBuffer buffer( sizeof(int) + sizeof(_classType) );
		buffer.serialize(std::string("CMessageChangePlayerClass"), true);
		// Usamos directamente write para evitar problemas con unsigned int
		buffer.write(&_classType, sizeof(_classType));

		return buffer;
	}
	
	//__________________________________________________________________

	void CMessageChangePlayerClass::deserialize(Net::CBuffer& buffer) {
		// Usamos directamente read para evitar problemas con unsigned int
		buffer.read(&_classType, sizeof(_classType));
	}
};
