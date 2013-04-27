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
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"

namespace Logic 
{
	IMP_FACTORY(CElevator);
	
	//---------------------------------------------------------
	
	bool CElevator::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("waitTime"))
			_waitTime= entityInfo->getIntAttribute("waitTime");

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
		_wait=true;
		_waitTimeInFinal=0;
		_active=false;
		_waitInFinal=false;
		_waitTime*=1000;
		_go=false;

		_directionInitial=(_positionInitial-_positionFinal);
		_directionFinal=(_positionFinal-_positionInitial);
		_directionInitial.normalise();
		_directionFinal.normalise();
		_toFinal=false;

	} // activate
	//---------------------------------------------------------



	bool CElevator::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CElevator::process(const std::shared_ptr<CMessage>& message)
	{
		switch(message->getMessageType())
		{
		case Message::TOUCHED:
			_go=true;
			break;
		}

	} // process
	//----------------------------------------------------------

	void CElevator::onFixedTick(unsigned int msecs)
	{
		Vector3 toDirection;
	
		//Timer para cuando llegamos arriba
		if(_waitInFinal)
			_waitTimeInFinal+=msecs;

		//Si estuvimos _launchTime
		if(_go && !_active){
			_active=true;
			_toFinal=true;
			_wait=false;
			_go=false;
		}
		//Si hemos pasado 2 segundos arriba, volvemos a bajar
		if(_active && _waitTimeInFinal>_waitTime){
			_toFinal=false;
			_wait=false;
			_waitInFinal=false;
			_waitTimeInFinal=0;
		}

		//Hacia la posicion final
		if(_toFinal && !_wait){
			float distanciaToFinal=_positionFinal.squaredDistance(_entity->getPosition());
			if(distanciaToFinal>=0){
				toDirection = _directionFinal * msecs * _velocity;
				//Por si nos pasasemos de la posición final
				if((_entity->getPosition()+toDirection).squaredDistance(_positionFinal)>distanciaToFinal){
					toDirection=(_positionFinal-_entity->getPosition());
					_wait=true;
					_waitInFinal=true;
					_waitTimeInFinal=0;
				}

				std::shared_ptr<CMessageKinematicMove> m = std::make_shared<CMessageKinematicMove>();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}
		//Hacia la posicion inicial
		else if(!_wait){
			float distanciaToInitial=_positionInitial.squaredDistance(_entity->getPosition());
			if(distanciaToInitial>=0){
				toDirection = _directionInitial* msecs * _velocity;
				//Por si nos pasasemos de la posición inicial
				if((_entity->getPosition()+toDirection).squaredDistance(_positionInitial)>distanciaToInitial){
					toDirection=(_positionInitial-_entity->getPosition());
					_wait=true;
					_active=false;
				}

				std::shared_ptr<CMessageKinematicMove> m = std::make_shared<CMessageKinematicMove>();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}






	} // tick
	//----------------------------------------------------------


} // namespace Logic


