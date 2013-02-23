/**
@file ElevatorTrigger.cpp

Contiene la implementación del componente que controla el movimiento de un ascensor.
 
@see Logic::CElevatorTrigger
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#include "ElevatorTrigger.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageKinematicMove.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"

namespace Logic 
{
	IMP_FACTORY(CElevatorTrigger);
	
	//---------------------------------------------------------
	
	bool CElevatorTrigger::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("positionInitial"))
			_positionInitial = entityInfo->getVector3Attribute("positionInitial");

		if(entityInfo->hasAttribute("positionFinal"))
			_positionFinal = entityInfo->getVector3Attribute("positionFinal");

		if(entityInfo->hasAttribute("link"))
			_entityLink = entityInfo->getStringAttribute("link");

		if(entityInfo->hasAttribute("velocity"))
			_velocity = entityInfo->getFloatAttribute("velocity");

		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CElevatorTrigger::activate()
	{
		IComponent::activate();
		
		_directionInitial=(_positionInitial-_positionFinal);
		_directionFinal=(_positionFinal-_positionInitial);
		_directionInitial.normalise();
		_directionFinal.normalise();
		_toFinal=false;

		_elevatorLink=Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_entityLink);
	} // activate
	//---------------------------------------------------------



	bool CElevatorTrigger::accept(CMessage *message)
	{
		return message->getMessageType() == Message::TOUCHED|| 
			message->getMessageType() == Message::UNTOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CElevatorTrigger::process(CMessage *message)
	{
		Logic::CMessageTouched *t;
		Logic::CMessageUntouched *u;
		switch(message->getMessageType())
		{
		case Message::TOUCHED:
			//std::cout << "CHOCO CON ASCENSOR Y PROCESO" << ((CMessageTouched*)message)->getEntity()->getName() << std::endl;
			t = new Logic::CMessageTouched();
			t->setEntity(getEntity());
			_elevatorLink->emitMessage(t);
			_toFinal=true;
			break;
		case Message::UNTOUCHED:
			//std::cout << "DESCHOCO CON ASCENSOR Y PROCESO" << std::endl;
			u = new Logic::CMessageUntouched();
			u->setEntity(getEntity());
			_elevatorLink->emitMessage(u);
			_toFinal=false;
			break;
		}

	} // process
	//----------------------------------------------------------

	void CElevatorTrigger::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		Vector3 toDirection;
		//Hacia la posicion final
		if(_toFinal){
			if(_entity->getPosition().distance(_positionFinal)>0.5){
				toDirection = _directionFinal * msecs * _velocity;
				Logic::CMessageKinematicMove* m = new Logic::CMessageKinematicMove();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}
		//Hacia la posicion inicial
		else{
			if(_entity->getPosition().distance(_positionInitial)>0.5){
				toDirection = _directionInitial * msecs * _velocity;
				Logic::CMessageKinematicMove* m = new Logic::CMessageKinematicMove();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}
	} // tick
	//----------------------------------------------------------


} // namespace Logic


