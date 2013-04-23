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
#include "Logic/Messages/MessageAudio.h"

#include "Logic/Messages/MessageCreateParticle.h"


namespace Logic 
{
	IMP_FACTORY(CJumper);
	
	//---------------------------------------------------------

	bool CJumper::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("force"))
			_force = entityInfo->getVector3Attribute("force");

		if(entityInfo->hasAttribute("audio"))
			_audio = entityInfo->getStringAttribute("audio");

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
		Logic::CMessageAudio *maudio=new Logic::CMessageAudio();

		switch(message->getMessageType())
		{
		case Message::TOUCHED:
			applyJump(((CMessageTouched*)message)->getEntity());
			maudio->setRuta(_audio);
			maudio->setId("jumper");
			maudio->setPosition(_entity->getPosition());
			maudio->setNotIfPlay(false);
			maudio->setIsPlayer(false);
			_entity->emitMessage(maudio);
			break;
		}

	} // process
	//---------------------------------------------------------

	
	//---------------------------------------------------------

	void CJumper::applyJump(CEntity *entity)
	{
		CMessageAddForcePlayer *m=new Logic::CMessageAddForcePlayer();
		m->setForce(_force+Vector3(0,5,0));
		entity->emitMessage(m);


	} // applyJump
	//---------------------------------------------------------

		//---------------------------------------------------------
	void CJumper::tick(unsigned int msecs) {
		IComponent::tick(msecs);



	} // tick

} // namespace Logic

