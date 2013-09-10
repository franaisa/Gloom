/**
@file ExplosionCompositor.cpp

Contiene la implementación del componente que gestiona la explosion del screamer.

@see Logic::CExplosionCompositor
@see Logic::IComponent

@author Javier Fernandez Villanueva
@date agoust, 2013
*/

#include "ExplosionCompositor.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddForcePlayer.h"
#include "Logic/Messages/MessageAudio.h"

#include "Logic/Messages/MessageCreateParticle.h"


namespace Logic 
{
	IMP_FACTORY(CExplosionCompositor);
	
	//---------------------------------------------------------

	bool CExplosionCompositor::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("time"))
		_time = entityInfo->getFloatAttribute("time");

		return true;

	} // spawn
	//---------------------------------------------------------

	bool CExplosionCompositor::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	//---------------------------------------------------------


	void CExplosionCompositor::process(const std::shared_ptr<CMessage>& message)
	{
		
	

	} // process
	
} // namespace Logic

