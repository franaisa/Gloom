/**
@file MagneticBullet.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CMagneticBullet
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Mayo, 2013
*/

// Logica
#include "MagneticBullet.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

// Fisica
#include "Logic/Messages/MessageTouched.h"

namespace Logic 
{
	IMP_FACTORY(CMagneticBullet);
	
	//---------------------------------------------------------
	
	
	bool CMagneticBullet::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		/*
		if(entityInfo->hasAttribute("colour")){
			Vector3 colour = entityInfo->getVector3Attribute("colour");
			_light->setColour(colour.x, colour.y, colour.z);
		}
		*/

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CMagneticBullet::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
		//return message->getMessageType() == Message::DAMAGED;
		
	} // accept
	
	//---------------------------------------------------------

	void CMagneticBullet::process(const std::shared_ptr<CMessage>& message)
	{
		
		switch(message->getMessageType())
		{
			case Message::TOUCHED:
				auto msg = std::static_pointer_cast<CMessageTouched>(message);
				impact(msg->getEntity());
		}
		

	} // process
	//----------------------------------------------------------
	
	void CMagneticBullet::setOwner(IComponent *owner)
	{
		_owner = owner;
	} // setOwner
	//----------------------------------------------------------

	void CMagneticBullet::impact(CEntity *impactEntity)
	{
		printf("Impacto con: %s", impactEntity->getName().c_str());
	} // impact
	//----------------------------------------------------------


/*
	void CMagneticBullet::onTick(unsigned int msecs)
	{
		
	} // tick
	//----------------------------------------------------------
*/


} // namespace Logic


