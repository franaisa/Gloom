/**
@file CoolDown.cpp

Contiene la implementación del componente que representa al coolDown.
 
@see Logic::CCoolDown

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "HardDeathClient.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"

#include "Logic/Messages/MessageCreateParticle.h"


namespace Logic {
	IMP_FACTORY(CHardDeathClient);

	CHardDeathClient::~CHardDeathClient() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CHardDeathClient::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpell::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute(_spellName + "ExplotionRadius") );

		_explotionRadius = entityInfo->getFloatAttribute(_spellName + "ExplotionRadius");
		return true;
	} // spawn
	//__________________________________________________________________

	void CHardDeathClient::spell(){
		
		std::shared_ptr<CMessageCreateParticle> particleMsg = std::make_shared<CMessageCreateParticle>();
		particleMsg->setPosition(_entity->getPosition());
		particleMsg->setParticle("ExplosionParticle");
		_entity->emitMessage(particleMsg);
	} // spell
	//__________________________________________________________________
		
	void CHardDeathClient::stopSpell(){
		

	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

