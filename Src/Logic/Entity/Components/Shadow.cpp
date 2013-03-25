/**
@file Shadow.cpp
 
@see Logic::CShadow
@see Logic::IComponent

@author 
@date Marzo, 2013
*/

#include "Shadow.h"

namespace Logic {
	IMP_FACTORY(CShadow);
	
	//__________________________________________________________________

	bool CShadow::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;


	} // spawn

} // namespace Logic

