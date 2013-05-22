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

	void CMessageCameraRoll::setRollDegrees(float fDegrees) {
		this->_fRollDegrees = fDegrees;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer CMessageCameraRoll::serialize() {
		Logic::TEntityID id = _entity->getEntityID();

		Net::CBuffer buffer( sizeof(int) + sizeof(id) );
		buffer.serialize(std::string("CMessageSetRelatedEntity"), true);
		buffer.serialize(id);
		
		return buffer;
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
