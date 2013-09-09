/**
@file SpellManagerServer.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CSpellManagerServer
@see Logic::IComponent

@Author Antonio Jesus Narvaez
@date September, 2013
*/

#include "SpellsManagerServer.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "AmplifyDamageServer.h"
#include "CoolDownServer.h"

namespace Logic 
{
	IMP_FACTORY(CSpellsManagerServer);

	//---------------------------------------------------------
	
	CSpellsManagerServer::CSpellsManagerServer() {
		
	}

	//---------------------------------------------------------

	bool CSpellsManagerServer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		/*
		assert( entityInfo->hasAttribute("primarySpell") && "Debe de tener campo primarySpell");
		assert( entityInfo->hasAttribute("secondarySpell") && "Debe de tener campo secondarySpell");
		
		_primarySpell = (SpellType::Enum)entityInfo->getIntAttribute("primarySpell");
		_secondarySpell = (SpellType::Enum)entityInfo->getIntAttribute("secondarySpell");


		entitySpawn = entity;
		mapSpawn = map;
		entityInfoSpawn = entityInfo;

		return true;
		*/

	} // spawn
	
	//---------------------------------------------------------

	void CSpellsManagerServer::onStart(){

		
	} // onStart
	//---------------------------------------------------------


} // namespace Logic

