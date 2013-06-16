/**
@file Interpolation.cpp

@author Francisco Aisa García
@author Ruben Mulero Guerrero
@date Febrero, 2013
*/

#include "MovementSnapshot.h"

#include "Logic/Entity/Entity.h"
#include "PhysicController.h"
#include "AvatarController.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

#include "Logic/Messages/MessageSyncPosition.h"
#include "Logic/Messages/MessageControl.h"
#include "Logic/Entity/Components/AvatarController.h"

#include "Logic/GameNetMsgManager.h"

#include <math.h>

#include <iostream>

namespace Logic  {
	
	IMP_FACTORY(CMovementSnapshot);

	//________________________________________________________________________

	bool CMovementSnapshot::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		return true;
	} // spawn

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CMovementSnapshot::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::CONTROL;
	} // accept

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CMovementSnapshot::process(const std::shared_ptr<CMessage>& message) {
		switch(message->getMessageType())
		{
		case Message::CONTROL:
			{
				std::shared_ptr<CMessageControl> controlMsg = std::static_pointer_cast<CMessageControl>(message);
				Control::ControlType type = controlMsg->getType();
				if(type == Control::LEFT_CLICK || type == Control::RIGHT_CLICK || type == Control::STOP_PRIMARY_SKILL || 
					type == Control::STOP_SECONDARY_SKILL || type == Control::USE_PRIMARY_SKILL ||
					type == Control::USE_SECONDARY_SKILL || type == Control::UNRIGHT_CLICK || type == Control::UNLEFT_CLICK ){

					CGameNetMsgManager::getSingletonPtr()->sendEntityMessage(message, _entity->getEntityID());
				}

			break;
			}
		}

	} // proces
	
	//________________________________________________________________________

	void CMovementSnapshot::onFixedTick(unsigned int msecs){
		std::shared_ptr<CMessageSyncPosition> msg = std::make_shared<CMessageSyncPosition>();
		msg->setPosition( _entity->getPosition() );
		msg->setOrientation( _entity->getOrientation() );
		CGameNetMsgManager::getSingletonPtr()->sendEntityMessage(msg, _entity->getEntityID());
	}
} // namespace Logic

