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

#include <math.h>

namespace Logic 
{
	IMP_FACTORY(CFloatingMovement);
	
	//---------------------------------------------------------
	void CFloatingMovement::estimateItemFloatingPos(Vector3& position, unsigned int msecs) {
		_currentOrbitalPos += _orbitalSpeed * msecs;
		if(_currentOrbitalPos > 6.283) _currentOrbitalPos = 0;
		position.y += sin(_currentOrbitalPos) * _orbitalOffset; 
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
			_orbitalOffset = entityInfo->getFloatAttribute("orbitalOffset");
		}

		if(entityInfo->hasAttribute("orbitalRotationSpeed")) {
			_orbitalRotationSpeed = entityInfo->getFloatAttribute("orbitalRotationSpeed");
		}

		return true;
	} // spawn


} // namespace Logic

