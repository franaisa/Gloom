#include "MessageCameraToEnemy.h"

#include "Logic/Entity/MessageFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageCameraToEnemy);

	CMessageCameraToEnemy::CMessageCameraToEnemy() : CMessage(Message::CAMERA_TO_ENEMY) {
		// Nada que hacer
	}//
	//----------------------------------------------------------


	CEntity* CMessageCameraToEnemy::getEnemy(){
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageCameraToEnemy::setEnemy(CEntity* entity){
		_entity=entity;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageCameraToEnemy::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(Logic::TEntityID));
		_tempBuffer->serialize(std::string("CMessageCameraToEnemy"),true);
		_tempBuffer->serialize(_entity->getEntityID());
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageCameraToEnemy::deserialize(Net::CBuffer& buffer) {
		TEntityID id;
        buffer.deserialize(id);

		_entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	}

};
