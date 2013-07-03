/**
@file SpellManagerServer.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CSpellManagerServer
@see Logic::IComponent

@Author Antonio Jesus Narvaez
@date July, 2013
*/

#include "SpellsManagerServer.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "AmplifyDamageServer.h"
#include "ComeBackServer.h"
#include "HungryServer.h"
#include "GravityServer.h"
#include "HardDeathServer.h"
#include "ShieldServer.h"
#include "CoolDownServer.h"
#include "ResurrectionServer.h"

namespace Logic 
{
	IMP_FACTORY(CSpellsManagerServer);

	//---------------------------------------------------------
	
	CSpellsManagerServer::CSpellsManagerServer() {
		
	}

	//---------------------------------------------------------

	bool CSpellsManagerServer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		assert( entityInfo->hasAttribute("primarySpell") && "Debe de tener campo primarySpell");
		assert( entityInfo->hasAttribute("secondarySpell") && "Debe de tener campo secondarySpell");
		
		_primarySpell = (SpellType::Enum)entityInfo->getIntAttribute("primarySpell");
		_secondarySpell = (SpellType::Enum)entityInfo->getIntAttribute("secondarySpell");


		entitySpawn = entity;
		mapSpawn = map;
		entityInfoSpawn = entityInfo;

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CSpellsManagerServer::onStart(){
	// El resto de las armas están desactivadas, ya que no las tenemos
		addComponent();
	} // onStart
	//---------------------------------------------------------

	void CSpellsManagerServer::addComponent(){
		IComponent *temp;
		std::string ID;
		int i;
		SpellType::Enum spell;
		for(i = 0, spell = _primarySpell; i<2 ;++i, spell = _secondarySpell){
			switch(spell){
				case SpellType::eAMPLIFY_DAMAGE: 
						temp = new CAmplifyDamageServer();
						ID = std::string("CAmplifyDamageServer");
				break;
				case SpellType::eCOME_BACK: 
						temp = new CComeBackServer();
						ID = std::string("CComeBackServer");
				break;
				case SpellType::eCOOLDOWN: 
						temp = new CCoolDownServer();
						ID = "CCoolDownServer";
				break;
				case SpellType::eGRAVITY: 
						temp = new CGravityServer();
						ID = "CGravityServer";
				break;
				case SpellType::eHARD_DEATH: 
						temp = new CHardDeathServer();
						ID = "CHardDeathServer";
				break;
				case SpellType::eHUNGRY: 
						temp = new CHungryServer();
						ID = "CHungryServer";
				break;
				case SpellType::eSHIELD: 
						temp = new CShieldServer();
						ID = "CShieldServer";
				break;
				case SpellType::eRESURECTION: 
						temp = new CResurrectionServer();
						ID = "CResurrectionServer";
				break;
			}
			temp->spawn(entitySpawn, mapSpawn, entityInfoSpawn);
			temp->activate();
			_entity->addComponent(temp,ID);
		}
		
	} // addComponent
	//---------------------------------------------------------


} // namespace Logic

