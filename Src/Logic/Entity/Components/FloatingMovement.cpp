/**
@file FloatingMovement.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "FloatingMovement.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

namespace Logic 
{
	IMP_FACTORY(CFloatingMovement);
	
	//---------------------------------------------------------
	void CFloatingMovement::estimateItemFloatingPos(Vector3& position, unsigned int msecs) {
		if(_goingUp) {
			position.y += _orbitalSpeed * msecs;
			if(position.y > _orbitalTopY)
				_goingUp = false;
		}
		else {
			position.y -= _orbitalSpeed * msecs;
			if(position.y < _orbitalBottomY)
				_goingUp = true;
		}
	}

	//---------------------------------------------------------
	void CFloatingMovement::estimateItemRotation(unsigned int msecs) {
		_orientation = _entity->getYaw();
		_orientation += _orbitalRotationSpeed * msecs;
	}

	//---------------------------------------------------------
	void CFloatingMovement::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		Vector3 newItemPos = _entity->getPosition();
		estimateItemFloatingPos(newItemPos, msecs);
		estimateItemRotation(msecs);

		_entity->setPosition(newItemPos);
		_entity->setYaw(_orientation);
		
	} // tick

	//---------------------------------------------------------
	bool CFloatingMovement::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		Vector3 itemPosition;

		if(entityInfo->hasAttribute("orbitalSpeed")) {
			_orbitalSpeed = entityInfo->getFloatAttribute("orbitalSpeed");
		}
		if(entityInfo->hasAttribute("position")) {
			itemPosition = entityInfo->getVector3Attribute("position");
		}

		if(entityInfo->hasAttribute("orbitalOffset")) {
			float orbitalOffset = entityInfo->getFloatAttribute("orbitalOffset");
			_orbitalTopY = itemPosition.y + (orbitalOffset / 2);
			_orbitalBottomY = itemPosition.y - (orbitalOffset / 2);
		}

		if(entityInfo->hasAttribute("orbitalRotationSpeed")) {
			_orbitalRotationSpeed = entityInfo->getFloatAttribute("orbitalRotationSpeed");
		}

		return true;
	} // spawn


} // namespace Logic

