/**
@file Hacksaw.cpp

Contiene la implementación del componente que controla el movimiento de un ascensor.
 
@see Logic::CHacksaw
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#include "Hacksaw.h"

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
	IMP_FACTORY(CHacksaw);
	
	//---------------------------------------------------------
	
	bool CHacksaw::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
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


	
	void CHacksaw::activate()
	{
		IComponent::activate();

		_directionInitial=(_positionInitial-_positionFinal);
		_directionFinal=(_positionFinal-_positionInitial);
		_directionInitial.normalise();
		_directionFinal.normalise();
		_toFinal=true;

	} // activate
	//---------------------------------------------------------


	bool CHacksaw::accept(CMessage *message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CHacksaw::process(CMessage *message)
	{
		Logic::CMessageDamaged* m;
		switch(message->getMessageType())
		{
		case Message::TOUCHED:
			m = new Logic::CMessageDamaged();
			m->setDamage(1000);
			m->setEnemy(_entity);
			((CMessageTouched*)message)->getEntity()->emitMessage(m);
			break;
		}

	} // process
	//----------------------------------------------------------

	void CHacksaw::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		Vector3 toDirection;

		//Hacia la posicion final
		if(_toFinal){
			float distanciaToFinal=(_positionFinal-_entity->getPosition()).absDotProduct(Vector3(1,1,1));
			if(distanciaToFinal>=0){
				toDirection = _directionFinal * msecs * _velocity;
				//Por si nos pasasemos de la posición final
				if(toDirection.absDotProduct(Vector3(1,1,1))>distanciaToFinal){
					toDirection=(_positionFinal-_entity->getPosition());
					_toFinal=false;
				}
				
				Logic::CMessageKinematicMove* m = new Logic::CMessageKinematicMove();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}
		//Hacia la posicion inicial
		else{
			float distanciaToInitial=(_positionInitial-_entity->getPosition()).absDotProduct(Vector3(1,1,1));
			if(distanciaToInitial>=0){
				toDirection = _directionInitial* msecs * _velocity;
				//Por si nos pasasemos de la posición inicial
				if(toDirection.absDotProduct(Vector3(1,1,1))>distanciaToInitial){
					toDirection=(_positionInitial-_entity->getPosition());
					_toFinal=true;
				}

				Logic::CMessageKinematicMove* m = new Logic::CMessageKinematicMove();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}


	} // tick
	//----------------------------------------------------------


} // namespace Logic


