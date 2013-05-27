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
	
	//________________________________________________________________________

	void CMovementSnapshot::onFixedTick(unsigned int msecs){
		std::shared_ptr<CMessageSyncPosition> msg = std::make_shared<CMessageSyncPosition>();
		msg->setTransform( _entity->getTransform() );
		CGameNetMsgManager::getSingletonPtr()->sendEntityMessage(msg, _entity->getEntityID());
	}
} // namespace Logic

