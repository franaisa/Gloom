/**
@file Hound.cpp
 
@see Logic::CHound
@see Logic::IComponent

@author 
@date Marzo, 2013
*/

#include "Hound.h"

namespace Logic {
	IMP_FACTORY(CHound);
	
	//__________________________________________________________________

	bool CHound::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;


	} // spawn

} // namespace Logic

