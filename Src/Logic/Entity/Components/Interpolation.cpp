/**
@file Interpolation.cpp

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "Interpolation.h"

#include "Logic/Entity/Entity.h"
#include "PhysicController.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

#include "Logic/Messages/MessageSyncPosition.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"

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
		// Indicar parametros de interpolacion (ñapeado de momento)
		_interpolating = false;
		_maxDistance = 15;
		_minDistance = 0.5f;
		_minYaw = 1;
		_maxYaw = 2.5f;
		_yawDifference = 0;
		_rotationSpeed = 0.2;
		return true;
	} // spawn

	//________________________________________________________________________

	bool CInterpolation::accept(CMessage *message) {
		return message->getMessageType() == Message::SYNC_POSITION ||
				message->getMessageType() == Message::CONTROL;
	} // accept
	
	//________________________________________________________________________

	void CInterpolation::tick(){
		//si no estamos interpolando, gl
		if(!_interpolating)
			return;
		//lo primero de todo, movemos la posición del servidor para poder interpolar con más exactitud
		moveServerPos();
		if(_canInterpolateMove){
			//calculamos la direccion en la que debemos interpolar
			Vector3 direction = _serverPos.getTrans();
			direction = direction - _entity->getPosition();
			direction = direction * Vector3(1,0,1);
			float distance = direction.length();
			direction = direction.normalisedCopy();
		
			//calculamos el movimiento que debe hacer el monigote, mucho mas lento del que debe hacer de normal
			direction = direction*_speed/2;
			Vector3 newPos = _entity->getPosition()+direction;
			//vemos a ver si hemos recorrido más de lo que deberíamos, y actuamos en consecuencia
			if(direction.length() > distance){
				_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(_serverPos.getTrans());
			}else{
				_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(newPos);
			}
			//si nos hemos quedado suficientemente cerquita, dejaremos de interpolar
			newPos = newPos - _serverPos.getTrans();
			if (newPos.length() < _minDistance)
				_canInterpolateMove = false;
		}//if
		if(_canInterpolateRotation){

			//si la diferencia es demasiado grande, lo movemos a pelo
			if(_yawDifference > _maxYaw || _yawDifference < _maxYaw*(-1)){
				_entity->yaw(_yawDifference);
				_yawDifference = 0;
			}

			//si la diferencia no es demasiado grande, interpolamos
			else if(_yawDifference > _minYaw || _yawDifference < _minYaw*(-1)){

				//nos movemos el raton
				if(_yawDifference > _rotationSpeed){

				}else if(_yawDifference < _rotationSpeed*(-1)){
					_entity->yaw(_rotationSpeed);

				}else{
					_entity->yaw(_yawDifference);
					_yawDifference = 0;
				}//if(_yawDifference > _rotationSpeed)

			}//if
			//si la diferencia es pequeña no hacemos nada
			_canInterpolateRotation = false;
		}//if(_canInterpolateRotation)

		//si hemos terminado de interpolar, lo dejamos
		if(!_canInterpolateRotation && !_canInterpolateMove){
			_interpolating = false;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::process(CMessage *message) {
		switch(message->getMessageType())
		{
		case Message::SYNC_POSITION:
			{
			// nos guardamos la posi que nos han dado por si tenemos que interpolar
			_serverPos = ((CMessageSyncPosition*)message)->getTransform();
			//calculo el ping que tengo ahora mismo
			_actualPing = ((CMessageSyncPosition*)message)->getTime();
			_actualPing = clock()+Logic::CServer::getSingletonPtr()->getDiffTime()-_actualPing;
			//calculamos la interpolacion
			calculateInterpolation();
			break;
			}
		case Message::CONTROL:
			{
			//calculamos la dirección del movimiento
			switch(((CMessageControl*)message)->getType()){
			case Control::WALK:
				_serverDirection+=Vector3(0,0,1);
				_canInterpolateMove = true;
				break;

			case Control::STRAFE_LEFT:
				_serverDirection+=Vector3(1,0,0);
				_canInterpolateMove = true;
				break;

			case Control::STRAFE_RIGHT:
				_serverDirection+=Vector3(-1,0,0);
				_canInterpolateMove = true;
				break;

			case Control::WALKBACK:
				_serverDirection+=Vector3(0,0,-1);
				_canInterpolateMove = true;
				break;

			case Control::STOP_WALK:
				_serverDirection+=Vector3(0,0,-1);
				break;

			case Control::STOP_WALKBACK:
				_serverDirection+=Vector3(0,0,1);
				break;

			case Control::STOP_STRAFE_LEFT:
				_serverDirection+=Vector3(-1,0,0);
				break;

			case Control::STOP_STRAFE_RIGHT:
				_serverDirection+=Vector3(1,0,0);
				break;

			case Control::MOUSE:
				Math::yaw(((CMessageMouse*)message)->getMouse()[0], _serverPos);;
				_canInterpolateRotation = true;
				break;

			}//switch

			break;
			}//case Message::CONTROL:

		}//switch

		//comprobamos si nos estamos moviendo, de manera que si no nos estamos moviendo no interpolamos
		if(_serverDirection == Vector3(0,0,0))
			_canInterpolateMove = false;

		//comprobamos si es el jugador local, de manera que si no lo es le decimos que puede interpolar 
		//siempre que deba hacerlo
		if(!_entity->isPlayer())
			_canInterpolateMove = true;
	} // process

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::moveServerPos(){
		Vector3 newPos = _serverPos.getTrans();
		//nueva posi = (old posi + direcion*orientacion)*velocidad
		newPos+=_serverDirection*_speed*Math::getDirection(_serverPos);
		_serverPos.setTrans(newPos);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::calculateInterpolation(){
		//calculamos la distancia que hay entre donde estoy y donde me dice el servidor que debería estar
		Vector3 serverPos = _serverPos.getTrans();
		serverPos = (serverPos - _entity->getPosition())*Vector3(1,0,1);
		float distance = serverPos.length();
		
		//calculo la distancia que habría recorrido trasladándome hacia la posi que me ha dado el servidor
		Vector3 direction = serverPos.normalisedCopy();
		float myDistance = direction.length() * _actualPing/CLOCKS_PER_SEC;
		//seteo la distancia real
		distance = distance-myDistance;
		std::cout << "distancia a la que esta" << _entity->getName() << " " << distance << std::endl;
		//si la distancia es mayor de maxDistance .. set transform por cojones
		if(distance > _maxDistance){
			_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(_serverPos.getTrans());
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
		//Guardamos el grado de desactualización del ratón
		_yawDifference = _entity->getYaw() - Math::getYaw(_serverPos);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace Logic

