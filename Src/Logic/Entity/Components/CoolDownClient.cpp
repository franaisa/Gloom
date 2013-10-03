/**
@file CoolDownClient.cpp

Contiene la implementación del componente que representa al coolDown.
 
@see Logic::CCoolDownClient

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "CoolDownClient.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

namespace Logic {
	IMP_FACTORY(CCoolDownClient);

	CCoolDownClient::~CCoolDownClient() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CCoolDownClient::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		/*
		Map::CEntity *tempEntity = CEntityFactory::getSingletonPtr()->getInfo(_spellName);

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( tempEntity->hasAttribute("PercentageCooldown") );

		_percentage = tempEntity->getFloatAttribute("PercentageCooldown");
		*/
		return true;
	} // spawn
	//__________________________________________________________________

	/*
	void CCoolDownClient::onStart(){
	
	} // onStart
	//__________________________________________________________________
	*/

	void CCoolDownClient::spell(){ 
		// feedback de cooldown reducio
		Graphics::CServer::getSingletonPtr()->getActiveScene()->createCompositor("ColdDownCompositor");
		Graphics::CServer::getSingletonPtr()->getActiveScene()->setCompositorVisible("ColdDownCompositor",true);
	} // spell
	//__________________________________________________________________
		
	void CCoolDownClient::stopSpell() { 
		// fin feedback de cooldown reducio
		Graphics::CServer::getSingletonPtr()->getActiveScene()->setCompositorVisible("ColdDownCompositor",false);
		Graphics::CServer::getSingletonPtr()->getActiveScene()->destroyCompositor("ColdDownCompositor");
		
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

