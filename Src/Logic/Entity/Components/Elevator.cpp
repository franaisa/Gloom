/**
@file Elevator.cpp

Contiene la implementación del componente que controla el movimiento de un ascensor.
 
@see Logic::CElevator
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#include "Elevator.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageKinematicMove.h"
#include "Logic/Messages/MessageTouched.h"

namespace Logic 
{
	IMP_FACTORY(CElevator);
	
	//---------------------------------------------------------
	
	bool CElevator::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("positionInitial"))
			_positionInitial = entityInfo->getVector3Attribute("positionInitial");

		if(entityInfo->hasAttribute("positionFinal"))
			_positionFinal = entityInfo->getVector3Attribute("positionFinal");

		if(entityInfo->hasAttribute("velocity"))
			_velocity = entityInfo->getFloatAttribute("velocity");

		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CElevator::activate()
	{
		IComponent::activate();
		
		_directionInitial=(_positionInitial-_positionFinal);
		_directionFinal=(_positionFinal-_positionInitial);
		_directionInitial.normalise();
		_directionFinal.normalise();
		_toFinal=false;

	} // activate
	//---------------------------------------------------------



	bool CElevator::accept(CMessage *message)
	{
		return message->getMessageType() == Message::CONTACT_ENTER ||
			message->getMessageType() == Message::CONTACT_EXIT;
	} // accept
	
	//---------------------------------------------------------

	void CElevator::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::CONTACT_ENTER:
			_toFinal=true;
			break;
		case Message::CONTACT_EXIT:
			_toFinal=false;
			break;
		}

	} // process
	//----------------------------------------------------------

	void CElevator::tick(unsigned int msecs)
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


