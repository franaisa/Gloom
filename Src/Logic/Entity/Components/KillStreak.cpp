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
#include "Logic/Messages/MessageAddLife.h"

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
		_iLevel = 0;

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
		
		++_iContKills;

		//Comprobacion de si hemos cambiado de nivel
		int iAntiguoNivel = _iLevel;
		SetLevel();

		if (iAntiguoNivel != _iLevel)
		{
			//Si hay cambio de nivel, comprobar el tipo del personaje y realizar su kill streak determinado (ver cabecera de la clase .h)
			//Voy a intentar de momento hacerlo todo en esta misma clase para no crear muchos componentes y dejarlo todo centralizado aquí (si se puede)
			//LaunchClassKillStreak();
		}
	}
	//----------------------------------------------------------


	void CKillStreak::SetLevel()
	{
		//seteamos el nivel
		if (_iContKills < _iKillsLevel1)
			_iLevel = 0;
		else if ((_iContKills >= _iKillsLevel1) && (_iContKills < _iKillsLevel2))
			_iLevel = 1;
		else if ((_iContKills >= _iKillsLevel2) && (_iContKills < _iKillsLevel3))
			_iLevel = 2;
		else if ((_iContKills >= _iKillsLevel3) && (_iContKills < _iKillsLevel4))
			_iLevel = 3;
		else //_iContKills >= 4
			_iLevel = 4;

		//std::cout << "Soy : " << _entity->getName() << " y mi nivel es : " << _iLevel << " y mi killstreak es " << _iContKills <<std::endl;
	}
	//----------------------------------------------------------


	void CKillStreak::LaunchClassKillStreak()
	{
		std::string type = this->getType(); 
		if ((type == "Screamer") || (type == "LocalScreamer"))
		{

		}
		else if ((type == "Shadow") || (type == "LocalShadow"))
		{
		
		}
		else if ((type == "Hound") || (type == "LocalHound"))
		{

		}
		else if ((type == "Archangel") || (type == "LocalArchangel"))
		{
			ArchangelKillStreak();
		}
	}


	void CKillStreak::ArchangelKillStreak()
	{
		switch (_iLevel)
		{
			case 1: //x3: Aumento de la vida.
			{
				std::cout << "Envio vida" << std::endl;
				std::shared_ptr<Logic::CMessageAddLife> messageLife = std::make_shared<Logic::CMessageAddLife>();
				messageLife->setAddLife(20);
				this->getEntity()->emitMessage(messageLife);
			}
			break;
			case 2:

			break;
			case 3:

			break;
			case 4:

			break;
		}
	}

	


} // namespace Logic


