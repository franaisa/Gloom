/**
@file Archangel.cpp
 
@see Logic::CArchangel
@see Logic::IComponent

@author 
@date Marzo, 2013
*/

#include "Archangel.h"

namespace Logic {
	IMP_FACTORY(CArchangel);
	
	//__________________________________________________________________

	bool CArchangel::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;


	} // spawn

} // namespace Logic

