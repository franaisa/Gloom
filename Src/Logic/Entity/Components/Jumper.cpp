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

		if(entityInfo->hasAttribute("power"))
			_power = entityInfo->getFloatAttribute("power");

		if(entityInfo->hasAttribute("velocity"))
			_velocity = entityInfo->getFloatAttribute("velocity");

		if(entityInfo->hasAttribute("direction"))
			_direction = entityInfo->getVector3Attribute("direction");

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


	bool CJumper::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	//---------------------------------------------------------


	void CJumper::process(const std::shared_ptr<CMessage>& message)
	{
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				std::shared_ptr<CMessageAudio> audioMsg = std::static_pointer_cast<CMessageAudio>(message);
				std::shared_ptr<CMessageTouched> touchedMsg = std::static_pointer_cast<CMessageTouched>(message);

				applyJump(touchedMsg->getEntity());
				audioMsg->setRuta(_audio);
				audioMsg->setId("jumper");
				audioMsg->setPosition(_entity->getPosition());
				audioMsg->setNotIfPlay(false);
				audioMsg->setIsPlayer(false);
				_entity->emitMessage(audioMsg);
				
				break;
			}
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

