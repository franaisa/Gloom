/**
@file UpdateClientPosition.cpp

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "UpdateClientPosition.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

#include "Logic/Messages/MessageSyncPosition.h"

#include <math.h>

namespace Logic 
{
	IMP_FACTORY(CUpdateClientPosition);

	//________________________________________________________________________

	void CUpdateClientPosition::tick(unsigned int msecs) {
		IComponent::tick(msecs);
		
		if(_timer > _syncPosTimeStamp) {
			std::shared_ptr<CMessageSyncPosition> msg = std::make_shared<CMessageSyncPosition>();
			msg->setTransform( _entity->getTransform() );
			msg->setTime(clock());
			_entity->emitMessage(msg);
			_timer = 0;
		}

		_timer += msecs;
	} // tick

	//________________________________________________________________________

	bool CUpdateClientPosition::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if( entityInfo->hasAttribute("syncPosTimeStamp") ) {
			// Convertimos a msecs
			_syncPosTimeStamp = entityInfo->getFloatAttribute("syncPosTimeStamp") * 1000;
		}

		return true;
	} // spawn


} // namespace Logic

