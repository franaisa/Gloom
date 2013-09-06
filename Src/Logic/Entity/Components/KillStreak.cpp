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
#include "Logic/Messages/MessageKillStreak.h"

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

		if(entityInfo->hasAttribute("KillsLevel1"))
			_iKillsLevel1= entityInfo->getIntAttribute("KillsLevel1"); //cargar desde fichero

		if(entityInfo->hasAttribute("KillsLevel2"))
			_iKillsLevel2 = entityInfo->getIntAttribute("KillsLevel2");

		if(entityInfo->hasAttribute("KillsLevel3"))
			_iKillsLevel3 = entityInfo->getIntAttribute("KillsLevel3");

		if(entityInfo->hasAttribute("KillsLevel4"))
			_iKillsLevel4 = entityInfo->getIntAttribute("KillsLevel4");


		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CKillStreak::onActivate()
	{
		//Al morir, y revivir, el jugador se pasa por este método de activación, así que se reseteao a 0 
		//el contador de killstreaks
		_iContKills = 0;

	} // activate
	//---------------------------------------------------------



	bool CKillStreak::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::KILL_STREAK;
	} // accept
	
	//---------------------------------------------------------

	void CKillStreak::process(const std::shared_ptr<CMessage>& message)
	{
		switch(message->getMessageType())
		{
			case Message::KILL_STREAK:
				dead(message);
			break;
		}

	} // process
	//----------------------------------------------------------

	void CKillStreak::onFixedTick(unsigned int msecs)
	{
		//std::cout << "Mi id es " << this->getEntity() << " y mi kill streak es " << _iContKills << std::endl;
	} // tick
	//----------------------------------------------------------

	void CKillStreak::dead(const std::shared_ptr<CMessage>& message)
	{		
		std::shared_ptr<CMessageKillStreak> mesKS = std::static_pointer_cast<CMessageKillStreak> (message);
		//std::cout << "ola ke ase! mi entity es: " << _entity->getEntityID() << " y me ha llegado en el mensaje " << mesKS->getKiller() << std::endl;

		++_iContKills;

		//std::cout << "Contador kill streak " << _iContKills << std::endl;
	}

} // namespace Logic


