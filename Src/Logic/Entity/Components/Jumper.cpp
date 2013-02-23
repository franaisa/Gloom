/**
@file Jumper.cpp

Contiene la implementación del componente que gestiona la aplicacion de una fuerza de salto al jugador.

@see Logic::CJumper
@see Logic::IComponent

*/

#include "Jumper.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddForcePlayer.h"




namespace Logic 
{
	IMP_FACTORY(CJumper);
	
	//---------------------------------------------------------

	bool CJumper::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("power"))
			_power = entityInfo->getFloatAttribute("power");

		if(entityInfo->hasAttribute("velocity"))
			_velocity = entityInfo->getFloatAttribute("velocity");

		if(entityInfo->hasAttribute("direction"))
			_direction = entityInfo->getVector3Attribute("direction");

		return true;

	} // spawn
	//---------------------------------------------------------

	void CJumper::activate()
	{
		IComponent::activate();
		
	} // activate
	//--------------------------------------------------------


	bool CJumper::accept(CMessage *message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	//---------------------------------------------------------


	void CJumper::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::TOUCHED:
			applyJump(((CMessageTouched*)message)->getEntity());
			break;
		}

	} // process
	//---------------------------------------------------------

	
	//---------------------------------------------------------

	void CJumper::applyJump(CEntity *entity)
	{
		CMessageAddForcePlayer *m=new Logic::CMessageAddForcePlayer();
		m->setPower(_power);
		m->setVelocity(_velocity);
		m->setDirection(_direction);
		entity->emitMessage(m);
	} // applyJump
	//---------------------------------------------------------

		//---------------------------------------------------------
	void CJumper::tick(unsigned int msecs) {
		IComponent::tick(msecs);

	} // tick

} // namespace Logic

