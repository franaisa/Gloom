/**
@file GravityServer.cpp

Contiene la implementación del componente que representa al gravity.
 
@see Logic::CGravityServer

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "GravityServer.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"



namespace Logic {
	IMP_FACTORY(CGravityServer);

	CGravityServer::~CGravityServer() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CGravityServer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;
		
		return true;
	} // spawn
	//__________________________________________________________________

	void CGravityServer::spell(){ 
		// Nada, ya que el movimiento va en el cliente

	} // spell
	//__________________________________________________________________
		
	void CGravityServer::stopSpell() { 
		
	} // stopSpell
	//__________________________________________________________________

	

	
} // namespace Logic

