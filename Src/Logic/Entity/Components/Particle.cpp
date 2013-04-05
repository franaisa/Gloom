/**
@file Particle.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CParticle
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Marzo, 2013
*/

#include "Particle.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Graphics/Particle.h"
#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include <OgreSceneManager.h>
#include "Logic/Server.h"

namespace Logic 
{
	IMP_FACTORY(CParticle);
	
	//---------------------------------------------------------
	
	bool CParticle::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		

		Vector3 position;
		if(entityInfo->hasAttribute("particlePosition"))
			position = entityInfo->getVector3Attribute("particlePosition");
		
		Vector3 direction;

		if(entityInfo->hasAttribute("particleDirection"))
			direction = entityInfo->getVector3Attribute("particleDirection");

		std::string particleName = entityInfo->getStringAttribute("particleName");
		
		_particle = new Graphics::CParticle(_entity->getName(), particleName);

		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CParticle::activate()
	{
		IComponent::activate();

	} // activate
	//---------------------------------------------------------



	bool CParticle::accept(CMessage *message)
	{
		return false;
		//return message->getMessageType() == Message::DAMAGED;
		
	} // accept
	
	//---------------------------------------------------------

	void CParticle::process(CMessage *message)
	{
		/*
		switch(message->getMessageType())
		{

		}
		*/

	} // process
	//----------------------------------------------------------
/*
	void CParticle::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		
	} // tick
	//----------------------------------------------------------
*/


} // namespace Logic


