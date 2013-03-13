/**
@file Interpolation.cpp

@author Francisco Aisa García
@author Ruben Mulero Guerrero
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
		_serverDirection = Vector3(0,0,0);
		_serverStrafeDirection = Vector3(0,0,0);
		return true;
	} // spawn

	//________________________________________________________________________

	bool CInterpolation::accept(CMessage *message) {
		return message->getMessageType() == Message::SYNC_POSITION ||
				message->getMessageType() == Message::CONTROL;
	} // accept
	
	//________________________________________________________________________

	void CInterpolation::tick(unsigned int msecs){
		IComponent::tick(msecs);
		//si no estamos interpolando, gl
		//std::cout << "interpolamos? -> " << _interpolating << std::endl;
		moveServerPos(msecs);
		if(!_interpolating)
			return;
		//lo primero de todo, movemos la posición del servidor para poder interpolar con más exactitud
		
		//std::cout << "puedo interpolar? " << _canInterpolateMove << std::endl;
		if(_canInterpolateMove){
			//calculamos la direccion en la que debemos interpolar
			Vector3 direction = _serverPos.getTrans();
			direction = direction - _entity->getPosition();
			direction = direction * Vector3(1,0,1);
			float distance = direction.length();
			float myDistance = distance * _actualPing/CLOCKS_PER_SEC;
			//seteo la distancia real
			distance = distance-myDistance;

			direction = direction.normalisedCopy();
		
			//calculamos el movimiento que debe hacer el monigote, mucho mas lento del que debe hacer de normal
			direction *=_speed/5*msecs;
			//Vector3 newPos = _entity->getPosition()+direction;
			//vemos a ver si hemos recorrido más de lo que deberíamos, y actuamos en consecuencia
			//std::cout << "server pos " << _serverPos.getTrans() << std::endl;
			//std::cout << "mi pos " << _entity->getPosition() << std::endl ;
			if(direction.length() > distance){
				_entity->getComponent<CPhysicController>("CPhysicController")->moveController(direction, msecs);
			}else{
				_entity->getComponent<CPhysicController>("CPhysicController")->moveController(direction, msecs);
			}
			//si nos hemos quedado suficientemente cerquita, dejaremos de interpolar
			Vector3 newPos = (_entity->getPosition() - _serverPos.getTrans())*Vector3(1,0,1);
			if (newPos.length() < _minDistance)
				_canInterpolateMove = false;
			//std::cout << "nueva pos " << newPos << std::endl ;
			//std::cout << "nueva pos lenght " << newPos.length() << std::endl << std::endl;
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
			CMessageSyncPosition* syncMsg = static_cast<CMessageSyncPosition*>(message);

			// nos guardamos la posi que nos han dado por si tenemos que interpolar
			_serverPos = syncMsg->getTransform();
			//calculo el ping que tengo ahora mismo
			_actualPing = clock()+Logic::CServer::getSingletonPtr()->getDiffTime()-syncMsg->getTime();
			//calculamos la interpolacion
			calculateInterpolation();

			break;
			}
		case Message::CONTROL:
			{
			//calculamos la dirección del movimiento
			switch(((CMessageControl*)message)->getType()){
			case Control::WALK:
				_serverDirection+=Vector3(1,0,1);
				_canInterpolateMove = true;
				break;

			case Control::STRAFE_LEFT:
				_serverStrafeDirection+=Vector3(1,0,1);
				_canInterpolateMove = true;
				break;

			case Control::STRAFE_RIGHT:
				_serverStrafeDirection+=Vector3(-1,0,-1);
				_canInterpolateMove = true;
				break;

			case Control::WALKBACK:
				_serverDirection+=Vector3(-1,0,-1);
				_canInterpolateMove = true;
				break;

			case Control::STOP_WALK:
				_serverDirection+=Vector3(-1,0,-1);
				break;

			case Control::STOP_WALKBACK:
				_serverDirection+=Vector3(1,0,1);
				break;

			case Control::STOP_STRAFE_LEFT:
				_serverStrafeDirection+=Vector3(-1,0,-1);
				break;

			case Control::STOP_STRAFE_RIGHT:
				_serverStrafeDirection+=Vector3(1,0,1);
				break;

			case Control::MOUSE:
				Math::yaw(((CMessageMouse*)message)->getMouse()[0], _serverPos);;
				_canInterpolateRotation = true;
				break;

			}//switch

			//std::cout << "serverDirection = " << _serverDirection << std::endl;
			//std::cout << "serverStrafeDirection = " << _serverStrafeDirection << std::endl << std::endl;

			break;
			}//case Message::CONTROL:

		}//switch
		//comprobamos si nos estamos moviendo, de manera que si no nos estamos moviendo no interpolamos
		if((_serverDirection == Vector3(0,0,0)) && (_serverStrafeDirection == Vector3(0,0,0)) ){
			_canInterpolateMove = false;
		}

		//comprobamos si es el jugador local, de manera que si no lo es le decimos que puede interpolar 
		//siempre que deba hacerlo
		if(!_entity->isPlayer())
			_canInterpolateMove = true;
	} // process

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::moveServerPos(unsigned int msecs){

		//primero movemos hacia adelante o hacia atrás
		if(_serverDirection != Vector3(0,0,0)){
			Vector3 newPos = _serverPos.getTrans();
			//nueva posi = (old posi + direcion*orientacion)*velocidad
			//std::cout << "server pos antes " << _serverPos.getTrans() << std::endl;
			newPos+=_serverDirection.normalisedCopy()*_speed*Math::getDirection(_serverPos)*msecs;
			//std::cout << "server pos mientras " << newPos << std::endl;
			_serverPos.setTrans(newPos);
		}
		
		//ahora nos movemos en la dirección del strafe
		if(_serverStrafeDirection != Vector3(0,0,0)){
			Matrix4 strafe = _serverPos;
			//std::cout << "direccion en INTERPOLATION 1 " << _serverStrafeDirection.normalisedCopy()*Math::getDirection(strafe) <<  std::endl;
			Math::yaw(Math::PI*0.5, strafe);
			Vector3 newPos = strafe.getTrans();
			//nueva posi = (old posi + direcion*orientacion)*velocidad
			//std::cout << "server pos antes " << _serverPos.getTrans() << std::endl;
			newPos+=_serverStrafeDirection.normalisedCopy()*_speed*Math::getDirection(strafe)*msecs;
			//std::cout << "server pos mientras " << newPos << std::endl;
			_serverPos.setTrans(newPos);
			//std::cout << "direccion en INTERPOLATION 2 " << _serverStrafeDirection.normalisedCopy()*Math::getDirection(strafe) <<  std::endl << std::endl;
		}
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
		std::cout << "distancia a la que esta" << _entity->getName() << " " << distance << "con ping " << _actualPing << "ms" << std::endl;
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
		}
		
		//Guardamos el grado de desactualización del ratón
		_yawDifference = _entity->getYaw() - Math::getYaw(_serverPos);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace Logic

