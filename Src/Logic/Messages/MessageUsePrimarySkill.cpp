#include "MessageUsePrimarySkill.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageUsePrimarySkill);

	//__________________________________________________________________
	
	CMessageUsePrimarySkill::CMessageUsePrimarySkill() : CMessage(Message::USE_PRIMARY_SKILL) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMessageUsePrimarySkill::~CMessageUsePrimarySkill() {
		// Nada que hacer
	}

	//__________________________________________________________________

	Net::CBuffer* CMessageUsePrimarySkill::serialize() {
		assert(_tempBuffer == NULL);

		// Tamaño de la cabecera (int)
		_tempBuffer = new Net::CBuffer( sizeof(int) );
		_tempBuffer->serialize(std::string("CMessageUsePrimarySkill"), true);

		return _tempBuffer;
	}
	
	//__________________________________________________________________

	void CMessageUsePrimarySkill::deserialize(Net::CBuffer& buffer) {
		// Este mensaje no tiene informacion que deserializar
	}
};
