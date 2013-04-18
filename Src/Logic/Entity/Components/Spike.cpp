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
#include "Logic/Messages/MessageKinematicMove.h"

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

		if(entityInfo->hasAttribute("positionInitial"))
			_positionInitial = entityInfo->getVector3Attribute("positionInitial");

		if(entityInfo->hasAttribute("positionFinal"))
			_positionFinal = entityInfo->getVector3Attribute("positionFinal");

		if(entityInfo->hasAttribute("velocity"))
			_velocity = entityInfo->getFloatAttribute("velocity");


		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CSpike::activate()
	{
		IComponent::activate();
		_timer=0;
		_active=false;
		_wait=true;
		_timeToShow*=1000;
		_timeToUnshow*=1000;

		_directionInitial=(_positionInitial-_positionFinal);
		_directionFinal=(_positionFinal-_positionInitial);
		_directionInitial.normalise();
		_directionFinal.normalise();
		_toFinal=false;

	} // activate
	//---------------------------------------------------------


	bool CSpike::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CSpike::process(const std::shared_ptr<CMessage>& message) {
		
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				std::shared_ptr<CMessageTouched> touchedMsg = std::static_pointer_cast<CMessageTouched>(message);
				std::shared_ptr<CMessageDamaged> damagedMsg = std::make_shared<CMessageDamaged>();

				damagedMsg->setDamage(1000);
				damagedMsg->setEnemy(_entity);
				touchedMsg->getEntity()->emitMessage(damagedMsg);
				
				break;
			}
		}

	} // process
	//----------------------------------------------------------

	void CSpike::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		Vector3 toDirection;
		_timer+=msecs;
		//Si ha pasado el tiempo mostramos los pinchos
		if(_timer>_timeToShow && !_active){
			_timer=0;
			_active=true;
			_toFinal=true;
			_wait=false;
		}
		//Si los pinchos estan fuera solamente duran X segundos
		if(_active && _timer>_timeToUnshow){
			_active=false;
			_timer=0;
			_toFinal=false;
			_wait=false;
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
				}

				std::shared_ptr<CMessageKinematicMove> m = std::make_shared<CMessageKinematicMove>();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}


	} // tick
	//----------------------------------------------------------


} // namespace Logic


