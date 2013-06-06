/**
@file Message.cpp

Contiene la implementación de la clase Message y sus clases hijas, que serán los mensajes. Es una colección de componentes.

@author Jose Antonio García Yáñez
@date Enero, 2013
*/

#include "Message.h"
#include "Logic/Server.h"

namespace Logic 
{

	CMessage::~CMessage() {
		Logic::CServer::getSingletonPtr()->MESSAGE_DESTRUCTOR_COUNTER += 1;
	}

	//----------------------------------------------------------

	CMessage::CMessage(TMessageType t) : _type(t) {
		// Nada que hacer
		Logic::CServer::getSingletonPtr()->MESSAGE_CONSTRUCTOR_COUNTER += 1;
	}
	
}

	
