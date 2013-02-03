/**
@file FloatingMovement.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "FloatingMovement.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CFloatingMovement);
	
	//---------------------------------------------------------
	void CFloatingMovement::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);



	} // tick

	bool CFloatingMovement::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("orbitalSpeed")) {
			_orbitalSpeed = entityInfo->getFloatAttribute("orbitalSpeed");
		}
		if(entityInfo->hasAttribute("orbitalOffset")) {
			_orbitalOffset = entityInfo->getFloatAttribute("orbitalOffset");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CFloatingMovement::accept(CMessage *message)
	{
		return true;
	} // accept
	
	//---------------------------------------------------------

	void CFloatingMovement::process(CMessage *message)
	{
		/*switch(message->getMessageType())
		{
		case Message::DAMAGED:
			{
				damaged( ((CMessageDamaged*)message)->getDamage());
				
			}
			break;
			case Message::ADD_LIFE:
			{
				addLife( ((CMessageAddLife*)message)->getAddLife());
				
			}
			break;
			case Message::ADD_SHIELD:
			{
				addShield( ((CMessageAddShield*)message)->getAddShield());
				
			}
			break;
		}*/

	} // process

} // namespace Logic

