/**
@file Screamer.cpp
 
@see Logic::CScreamer
@see Logic::IComponent

@author 
@date Marzo, 2013
*/

#include "Screamer.h"

namespace Logic {

	IMP_FACTORY(CScreamer);
	
	//__________________________________________________________________

	bool CScreamer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;


	} // spawn

} // namespace Logic

