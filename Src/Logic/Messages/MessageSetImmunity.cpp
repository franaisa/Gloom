#include "MessageSetImmunity.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSetImmunity);

	//__________________________________________________________________

	CMessageSetImmunity::CMessageSetImmunity() : CMessage(Message::SET_IMMUNITY) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMessageSetImmunity::~CMessageSetImmunity() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CMessageSetImmunity::isImmune() {
		return _isImmune;
	}

	//__________________________________________________________________

	void CMessageSetImmunity::setImmunity(bool isImmune){
		this->_isImmune = isImmune;
	}

	//__________________________________________________________________
		
	Net::CBuffer* CMessageSetImmunity::serialize() {
		assert(_tempBuffer == NULL);

		// Tamaño de la cabecera (int) + attributo de immunidad (bool)
		_tempBuffer = new Net::CBuffer( sizeof(int) + sizeof(bool) );
		_tempBuffer->serialize( std::string("CMessageSetImmunity"), true );
		// Para evitar problemas con la serializacion de bool lo hacemos a mano
		_tempBuffer->write(&_isImmune, sizeof(bool));
		
		return _tempBuffer;
	}
	
	//__________________________________________________________________

	void CMessageSetImmunity::deserialize(Net::CBuffer& buffer) {
		// Para evitar problemas con la deserializacion de bool lo hacemos a mano
		buffer.read(&_isImmune, sizeof(bool));
	}

};
