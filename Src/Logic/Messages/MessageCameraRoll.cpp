#include "MessageCameraRoll.h"

#include "Logic/Entity/MessageFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageCameraRoll);

	CMessageCameraRoll::CMessageCameraRoll() : CMessage(Message::CAMERA_ROLL) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	CMessageCameraRoll::~CMessageCameraRoll() {
		// Nada que hacer
	}
	//----------------------------------------------------------

	void CMessageCameraRoll::setRollDegrees(int iDegrees) {
		this->_iRollDegrees = iDegrees;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageCameraRoll::serialize() {
		assert(_tempBuffer == NULL);

		Logic::TEntityID id = _entity->getEntityID();

		_tempBuffer = new Net::CBuffer( sizeof(int) + sizeof(id) );
		_tempBuffer->serialize(std::string("CMessageSetRelatedEntity"), true);
		_tempBuffer->serialize(id);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageCameraRoll::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
		// Por problemas con enumerados utilizamos directamente
		// el read en vez del deserialize
		buffer.read( &id, sizeof(id) );

		_entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	}

};
