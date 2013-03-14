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

		if(entityInfo->hasAttribute("launchTime"))
			_launchTime = entityInfo->getIntAttribute("launchTime");

		if(entityInfo->hasAttribute("waitTime"))
			_waitTime = entityInfo->getIntAttribute("waitTime");

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
		_timer=0;
		_waitTimeInFinal=0;
		_active=false;
		_waitInFinal=false;
		_wait=true;
		_launchTime*=1000;
		_waitTime*=1000;
		_touching=false;

		
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
			_touching=true;
			break;
		case Message::UNTOUCHED:
			_touching=false;
			break;
		}

	} // process
	//----------------------------------------------------------

	void CElevatorTrigger::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		Logic::CMessageTouched *t;
		Logic::CMessageUntouched *u;
		Vector3 toDirection;
		//Activación del ascensor solo si has pasado el _launchTime encima (tocando el trigger) y no estamos en un recorrido
		if(!_active){
			if(_touching)
				_timer+=msecs;
			else
				_timer=0;
		}
		//Timer para cuando llegamos arriba
		if(_waitInFinal)
			_waitTimeInFinal+=msecs;

		//Si estuvimos _launchTime
		if(_timer>_launchTime && !_active){
			std::cout << "DESPEGAMOS trigger" << std::endl;
			_active=true;
			_toFinal=true;
			_wait=false;
			//Mensaje para que la parte física haga el camino también.
			t = new Logic::CMessageTouched();
			t->setEntity(getEntity());
			_elevatorLink->emitMessage(t);
 
		}
		//Si hemos pasado 2 segundos arriba, volvemos a bajar
		if(_active && _waitTimeInFinal>_waitTime){
			std::cout << "BAJAMOS trigger" << std::endl;
			_timer=0;
			_toFinal=false;
			_wait=false;
			_waitInFinal=false;
			_waitTimeInFinal=0;
		}

		//Hacia la posicion final
		if(_toFinal && !_wait){
			float distanciaToFinal=(_positionFinal-_entity->getPosition()).absDotProduct(Vector3(1,1,1));
			if(distanciaToFinal>=0){
				toDirection = _directionFinal * msecs * _velocity;
				//Por si nos pasasemos de la posición final
				if(toDirection.absDotProduct(Vector3(1,1,1))>distanciaToFinal){
					toDirection=(_positionFinal-_entity->getPosition());
					_wait=true;
					_waitInFinal=true;
					_waitTimeInFinal=0;
				}
				Logic::CMessageKinematicMove* m = new Logic::CMessageKinematicMove();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}
		//Hacia la posicion inicial
		else if(!_wait){
			float distanciaToInitial=(_positionInitial-_entity->getPosition()).absDotProduct(Vector3(1,1,1));
			if(distanciaToInitial>=0){
				toDirection = _directionInitial* msecs * _velocity;
				//Por si nos pasasemos de la posición inicial
				if(toDirection.absDotProduct(Vector3(1,1,1))>distanciaToInitial){
					toDirection=(_positionInitial-_entity->getPosition());
					_wait=true;
					_active=false;
					_timer=0;
				}

				Logic::CMessageKinematicMove* m = new Logic::CMessageKinematicMove();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}

	} // tick
	//----------------------------------------------------------


} // namespace Logic


