/**
@file UpdateClientPosition.cpp

@author Francisco Aisa García
@author Rubén Mulero Guerrero
@date Febrero, 2013
*/

#include "UpdateClientPosition.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageSyncPosition.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/GameNetMsgManager.h"

namespace Logic 
{
	IMP_FACTORY(CUpdateClientPosition);


////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CUpdateClientPosition::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::CONTROL ||
			message->getMessageType() == Message::SYNC_POSITION;
	} // accept

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CUpdateClientPosition::process(const std::shared_ptr<CMessage>& message) {
		switch(message->getMessageType())
		{
		case Message::SYNC_POSITION:
				updateClientPosition(std::static_pointer_cast<CMessageSyncPosition>(message));
			break;
		case Message::CONTROL:
				sendACKMessage(std::static_pointer_cast<CMessageControl>(message)->getSeqNumber());
			break;
		}

	} // proces

////////////////////////////////////////////////////////////////////////////////////////////////////////


	bool CUpdateClientPosition::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if( entityInfo->hasAttribute("syncPosTimeStamp") ) {
			// Convertimos a msecs
			_syncPosTimeStamp = entityInfo->getFloatAttribute("syncPosTimeStamp") * 1000;
		}
		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");

		return true;
	} // spawn


	void CUpdateClientPosition::sendACKMessage(unsigned int sequenceNumber){
		std::shared_ptr<CMessageSyncPosition> ack = std::make_shared<CMessageSyncPosition>();
		Vector3 position = _entity->getPosition();
//		ack->setPosition( position );
//		ack->setSeq(sequenceNumber);

		//std::cout << "mensaje " << sequenceNumber << " resultado " << _entity->getComponent<CAvatarController>("CAvatarController")->getVelocity() << std::endl;

		CGameNetMsgManager::getSingletonPtr()->sendMessageToOne(ack, _entity->getEntityID(), _entity->getEntityID());
	}


	void CUpdateClientPosition::updateClientPosition(const std::shared_ptr<CMessageSyncPosition> &message){
	
		_physicController->setPhysicPosition(message->getPosition());
		_entity->setOrientation(message->getOrientation());
	}

} // namespace Logic

