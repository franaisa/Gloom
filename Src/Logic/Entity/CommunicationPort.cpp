/**
@file CommunicationPort.cpp

Contiene la implementación de la clase que se encarga del intercambio 
de los mensajes.

@see Logic::CCommunicationPort

@author David Llansó García
@author Francisco Aisa García
@date Abril, 2013
*/

#include "CommunicationPort.h"

namespace Logic {

	CCommunicationPort::CCommunicationPort() {
		// Nada que hacer
	}

	//__________________________________________________________________

	CCommunicationPort::~CCommunicationPort() {
		// Realmente esto no es necesario
		_messages.clear();

	} // ~CCommunicationPort
	
	//__________________________________________________________________

	bool CCommunicationPort::set(const std::shared_ptr<CMessage>& message) {
		bool accepted = accept(message);
		if(accepted) {
			_messages.push_back(message);
		}

		return accepted;
	} // set
	
	//__________________________________________________________________

	// Por defecto retornamos false. La idea es que los hijos redefinan este método
	bool CCommunicationPort::accept(const std::shared_ptr<CMessage>& message) {
		return false;
	}

	//__________________________________________________________________

	void CCommunicationPort::process(const std::shared_ptr<CMessage>& message) {
		// Debe ser redefinido por los hijos
	}

	//__________________________________________________________________

	bool CCommunicationPort::processMessages() {
		CMessageList::const_iterator it = _messages.begin();
		bool success = ( it != _messages.end() );
		for(; it != _messages.end(); ++it) {
			process(*it);
		}

		_messages.clear();
		return success;
	} // processMessages

	//__________________________________________________________________

	void CCommunicationPort::clearMessages() {
		_messages.clear();
	}

} // namespace Logic
