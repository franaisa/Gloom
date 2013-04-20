/**
@file NetConnector.h

Contiene la implementación del componente que reenvia mensajes por la red.

@see Logic::CNet
@see Logic::IComponent

@author David Llansó
@author Francisco Aisa García
@date Febrero, 2013
*/

#include "NetConnector.h"
#include <cassert>
#include <sstream>

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/Message.h"

#include "Net/Manager.h"
#include "Logic/GameNetMsgManager.h"
#include "Net/buffer.h"

#include <iostream>

#define BS_STACK_ALLOC_SIZE 128

namespace Logic {
		
	IMP_FACTORY(CNetConnector);

	bool CNetConnector::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {

		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		// NET: Procesamos la lista con los nombres de los mensajes...
		if (entityInfo->hasAttribute("msgList")) {
			std::istringstream mgsTypeList(entityInfo->getStringAttribute("msgList"));

			// Para cada cadena entre comas...
			do {
				std::string MsgTypeName;
				std::getline(mgsTypeList, MsgTypeName, ','); // linea entre delimitadores
				
				std::istringstream str(MsgTypeName);     // wrappeamos cadena como Input Stream
				do {									// Le quitamos los espacios...
					std::getline(str, MsgTypeName, ' ');  // linea entre espacios
				} while (MsgTypeName.size() == 0 && !str.eof());

				// ... y registramos el tipo en el set
				_forwardedMsgTypes.insert((Logic::TMessageType)atoi(MsgTypeName.c_str()));
			} while (!mgsTypeList.eof());
		}

		// Obtenemos los milisegundos que se esperan entre envios 
		// de mensajes del mismo tipo
		if (entityInfo->hasAttribute("blockedTime")) 
			_timeOfBlocking = entityInfo->getIntAttribute("blockedTime");

		return true;
	}

	bool CNetConnector::accept(const std::shared_ptr<CMessage>& message) {
		// TODO Vemos si es uno de los mensajes que debemos trasmitir 
		// por red. Para eso usamos la lista de mensajes que se ha
		// leido del mapa.
		// Vemos si es uno de los mensajes que debemos trasmitir 
		// por red.
		Logic::TMessageType msgType = message->getMessageType();
		if( _forwardedMsgTypes.find(msgType) != _forwardedMsgTypes.end() ) {
			// Grano fino, en vez de aceptar el mensaje directamente
			// solo se retransmitirá por la red si no se ha transmitido 
			// hace poco (_timeOfBlocking milisegundos) un mensaje del 
			// mismo tipo
			if(_timeToUnblockMsgDelivery.count(msgType) == 0) { // TODO probar sin ajuste fino qué tal va de fino :P
				if(_timeOfBlocking)
					_timeToUnblockMsgDelivery.insert(TTimeToUnblockMsgDeliveryPair(msgType, _timeOfBlocking));
				return true;
			}// TODO no hace falta ir descontando tiempo y eliminar el par en algún momento?
		}

		return false;

	} // accept
		
	//---------------------------------------------------------------------------------

	void CNetConnector::process(const std::shared_ptr<CMessage>& message) {
		// TODO Es un mensaje para enviar por el tubo.
		// Lo enviamos por la red usando el front-end CGameNetMsgManager
		Logic::TMessageType msgType = message->getMessageType();
		if(msgType == Logic::Message::CAMERA_TO_ENEMY) {
			std::cout << "Recibido un mensaje de tipo camera to enemy" << std::endl;
			std::cout << "menssaje de avatar controller ENVIADO" << std::endl;
			
			if( _forwardedMsgTypes.find(msgType) != _forwardedMsgTypes.end() ) {
				std::cout << "El mensaje se encuentra en nuestra lista de mensajes, por lo tanto lo enviamos" << std::endl;
			}

			Logic::TEntityID id = _entity->getEntityID();
			
		}

		CGameNetMsgManager::getSingletonPtr()->
			sendEntityMessage(message, _entity->getEntityID());

	} // process
		
	//---------------------------------------------------------------------------------
		
	void CNetConnector::tick(unsigned int msecs) {
	}

} // namespace Logic
