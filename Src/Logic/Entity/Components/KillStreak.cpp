/**
@file Elevator.h

Contiene la declaración del componente que controla el componente de los kill streak

@see Logic::CKillStreak
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Agosto, 2013
*/

#include "KillStreak.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessagePlayerDead.h"

#include "Logic/Messages/MessageKinematicMove.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"

namespace Logic 
{
	IMP_FACTORY(CKillStreak);
	
	//---------------------------------------------------------
	
	bool CKillStreak::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		/*if(entityInfo->hasAttribute("waitTime"))
			_waitTime= entityInfo->getIntAttribute("waitTime");

		if(entityInfo->hasAttribute("positionInitial"))
			_positionInitial = entityInfo->getVector3Attribute("positionInitial");

		if(entityInfo->hasAttribute("positionFinal"))
			_positionFinal = entityInfo->getVector3Attribute("positionFinal");

		if(entityInfo->hasAttribute("velocity"))
			_velocity = entityInfo->getFloatAttribute("velocity");*/



		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CKillStreak::onActivate()
	{
		_iContKills = 0;

	} // activate
	//---------------------------------------------------------



	bool CKillStreak::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::PLAYER_DEAD;
	} // accept
	
	//---------------------------------------------------------

	void CKillStreak::process(const std::shared_ptr<CMessage>& message)
	{
		switch(message->getMessageType())
		{
			case Message::PLAYER_DEAD:
				dead(message);
			break;
		}

	} // process
	//----------------------------------------------------------

	void CKillStreak::onFixedTick(unsigned int msecs)
	{

	} // tick
	//----------------------------------------------------------

	void CKillStreak::dead(const std::shared_ptr<CMessage>& message)
	{		
		std::shared_ptr<CMessagePlayerDead> playerDeadMsg = std::static_pointer_cast<CMessagePlayerDead> (message);
		CEntity* entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(playerDeadMsg->getKiller());

		if (entity)
		{
			if (entity->getName().compare(_entity->getName()) == 0)
				std::cout << "YO SOY EL ASESINO! " << std::endl;
			else
				std::cout << "El asesino es: " << entity->getName() << std::endl;
		}			
	}

} // namespace Logic


