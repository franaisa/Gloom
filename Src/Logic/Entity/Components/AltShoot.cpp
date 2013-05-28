/**
@file AltShoot.cpp


@see Logic::CAltShoot
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Febrero, 2013
*/

#include "AltShoot.h"

namespace Logic {
	
	CAltShoot::CAltShoot() {

	}

	CAltShoot::~CAltShoot() {

	}

	//__________________________________________________________________

	bool CAltShoot::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;
	}

	bool CAltShoot::accept(const std::shared_ptr<CMessage>& message) {
		return false;
	}

	void CAltShoot::process(const std::shared_ptr<CMessage>& message) {

	}

	//__________________________________________________________________

	void CAltShoot::onTick(unsigned int msecs) {
	
	}

	void CAltShoot::onAvailable() {

	}

} // namespace Logic

