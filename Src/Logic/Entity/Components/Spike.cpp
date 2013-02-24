/**
@file Spike.cpp

Contiene la implementación del componente que controla el movimiento de un ascensor.
 
@see Logic::CSpike
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#include "Spike.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageDeactivate.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageTouched.h"

namespace Logic 
{
	IMP_FACTORY(CSpike);
	
	//---------------------------------------------------------
	
	bool CSpike::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("timeToShow"))
			_timeToShow = entityInfo->getIntAttribute("timeToShow");

		if(entityInfo->hasAttribute("timeToUnshow"))
			_timeToUnshow = entityInfo->getIntAttribute("timeToUnshow");

		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CSpike::activate()
	{
		IComponent::activate();
		_timer=0;
		_active=false;
		_timeToShow*=1000;
		_timeToUnshow*=1000;

	} // activate
	//---------------------------------------------------------


	bool CSpike::accept(CMessage *message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CSpike::process(CMessage *message)
	{
		Logic::CMessageDamaged* m;
		switch(message->getMessageType())
		{
		case Message::TOUCHED:
			m = new Logic::CMessageDamaged();
			m->setDamage(1000);
			m->setEnemy(_entity);
			std::cout << ((CMessageTouched*)message)->getEntity()->getName() << std::endl;
			((CMessageTouched*)message)->getEntity()->emitMessage(m);
			break;
		}

	} // process
	//----------------------------------------------------------

	void CSpike::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		_timer+=msecs;
		//Si ha pasado el tiempo mostramos los pinchos
		if(_timer>_timeToShow && !_active){
			Logic::CMessageActivate *m=new Logic::CMessageActivate();
			_entity->emitMessage(m);
			_timer=0;
			_active=true;
		}
		//Si los pinchos estan fuera solamente duran X segundos
		if(_active && _timer>_timeToUnshow){
			Logic::CMessageDeactivate *m=new Logic::CMessageDeactivate();
			_entity->emitMessage(m);
			_active=false;
			_timer=0;
		}
	} // tick
	//----------------------------------------------------------


} // namespace Logic


