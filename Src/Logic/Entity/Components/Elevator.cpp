/**
@file Elevator.cpp

Contiene la implementación del componente que controla el movimiento de un ascensor.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Elevator.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageKinematicMove.h"


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

		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CElevator::activate()
	{
		IComponent::activate();
		

	} // activate
	//---------------------------------------------------------



	bool CElevator::accept(CMessage *message)
	{
		return message->getMessageType() == Message::ELEVATOR_INITIAL || 
			message->getMessageType() == Message::ELEVATOR_FINAL;
	} // accept
	
	//---------------------------------------------------------

	void CElevator::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::ELEVATOR_INITIAL:
			
			break;
		case Message::ELEVATOR_FINAL:
			
			break;
		}

	} // process
	//----------------------------------------------------------

	void CElevator::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		
	} // tick
	//----------------------------------------------------------

	void CElevator::toInitial(){
		
	}// toInitial
	//----------------------------------------------------------

	void CElevator::toFinal(){
		
	}// toInitial
	//----------------------------------------------------------

} // namespace Logic


