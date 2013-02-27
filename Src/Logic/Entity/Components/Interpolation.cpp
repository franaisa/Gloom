/**
@file Interpolation.cpp

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "Interpolation.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

#include "Logic/Messages/MessageSyncPosition.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"

#include <math.h>

#include <iostream>

namespace Logic  {
	
	IMP_FACTORY(CInterpolation);

	//________________________________________________________________________

	bool CInterpolation::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		if(entityInfo->hasAttribute("speed"))
			_speed = entityInfo->getFloatAttribute("speed");
		// Indicar parametros de interpolacion
		_interpolating = false;
		_maxDistance = 5;
		_minDistance = 1;
		return true;
	} // spawn

	//________________________________________________________________________

	bool CInterpolation::accept(CMessage *message) {
		return message->getMessageType() == Message::SYNC_POSITION;
	} // accept
	
	//________________________________________________________________________

	void CInterpolation::tick(){
		//si no estamos interpolando, gl
		if(!_interpolating)
			return;

		//calculamos la direccion en la que debemos interpolar
		Vector3 direction = _serverPos.getTrans();
		direction = direction - _entity->getPosition();
		float distance = direction.length();
		direction.normalisedCopy();
		
		//calculamos el movimiento que debe hacer el monigote, mucho mas lento del que debe hacer de normal
		direction = direction*_speed/3;
		Vector3 newPos = _entity->getPosition()+direction;
		//vemos a ver si hemos recorrido más de lo que deberíamos, y actuamos en consecuencia
		if(direction.length() > distance){
			Logic::CMessageSetPhysicPosition *m = new Logic::CMessageSetPhysicPosition();
			m->setPosition(_serverPos.getTrans());
			_entity->emitMessage(m);
		}else{

			Logic::CMessageSetPhysicPosition *m = new Logic::CMessageSetPhysicPosition();
			m->setPosition(newPos);
			_entity->emitMessage(m);

		}
		//si nos hemos quedado suficientemente cerquita, dejaremos de interpolar
		newPos = newPos - _serverPos.getTrans();
		if (newPos.length() < _minDistance)
			_interpolating = false;
	}

	//________________________________________________________________________

	void CInterpolation::process(CMessage *message) {
		switch(message->getMessageType())
		{
		case Message::SYNC_POSITION:
			// nos guardamos la posi que nos han dado por si tenemos que interpolar
			_serverPos = ((CMessageSyncPosition*)message)->getTransform();

			//calculamos la distancia que hay entre donde estoy y donde me dice el servidor que debería estar
			Vector3 serverPos = _serverPos.getTrans();
			serverPos = serverPos - _entity->getPosition();
			float distance = serverPos.length();

			//calculo el ping que tengo ahora mismo
			_actualPing = ((CMessageSyncPosition*)message)->getTime();
			_actualPing = clock()+Logic::CServer::getSingletonPtr()->getDiffTime()-_actualPing;

			//calculo la distancia que habría recorrido trasladándome hacia la posi que me ha dado el servidor
			Vector3 direction = serverPos.normalisedCopy();
			float myDistance = direction.length() * _actualPing/1000;

			//seteo la distancia real
			distance = distance-myDistance;
			std::cout << "distance -> " << distance << std::endl;
			//si la distancia es mayor de maxDistance .. set transform por cojones
			if(distance > _maxDistance){
				Logic::CMessageSetPhysicPosition *m = new Logic::CMessageSetPhysicPosition();
				m->setPosition(_serverPos.getTrans());
				_entity->emitMessage(m);
				//Movemos la orientacion logica/grafica
				Matrix3 tmpMatrix;
				_serverPos.extract3x3Matrix(tmpMatrix);
				_entity->setOrientation(tmpMatrix);
				return;
			}
			//si la distancia es mayor que min distance y menor que maxDistance interpolamos
			else if(distance > _minDistance){
				_interpolating = true;
				return;
			}

			//ahora veríamos la interpolación, pero de las rotaciones
			
			
			
			break;
		}
	} // process

} // namespace Logic

