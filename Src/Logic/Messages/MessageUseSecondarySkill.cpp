#include "MessageUseSecondarySkill.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageUseSecondarySkill);

	//__________________________________________________________________
	
	CMessageUseSecondarySkill::CMessageUseSecondarySkill() : CMessage(Message::USE_SECONDARY_SKILL) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMessageUseSecondarySkill::~CMessageUseSecondarySkill() {
		// Nada que hacer
	}

	//__________________________________________________________________

	Net::CBuffer* CMessageUseSecondarySkill::serialize() {
		assert(_tempBuffer == NULL);

		// Tamaño de la cabecera (int)
		_tempBuffer = new Net::CBuffer( sizeof(int) );
		_tempBuffer->serialize(std::string("CMessageUseSecondarySkill"), true);

		return _tempBuffer;
	}
	
	//__________________________________________________________________

	void CMessageUseSecondarySkill::deserialize(Net::CBuffer& buffer) {
		// Este mensaje no tiene informacion que deserializar
	}
};
