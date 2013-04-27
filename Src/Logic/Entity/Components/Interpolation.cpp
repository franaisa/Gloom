/**
@file Interpolation.cpp

@author Francisco Aisa García
@author Ruben Mulero Guerrero
@date Febrero, 2013
*/

#include "Interpolation.h"

#include "Logic/Entity/Entity.h"
#include "PhysicController.h"
#include "AvatarController.h"
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
		// Indicar parametros de interpolacion (ñapeado de momento)
		_interpolating = false;
		_maxDistance = 15;
		_minDistance = 0.25f;
		_minYaw = 1.5;
		_maxYaw = 15;
		_yawDifference = 0;
		_rotationSpeed = 0.2;
		_serverDirection = Vector3(0,0,0);
		_distance = 0;
		return true;
	} // spawn

	//________________________________________________________________________

	bool CInterpolation::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::SYNC_POSITION;
	} // accept
	
	//________________________________________________________________________

	void CInterpolation::onTick(unsigned int msecs){
		_msecs = msecs;

		//si no estamos interpolando, gl
		if(!_interpolating)
			return;

		//lo primero de todo, movemos la posición del servidor para poder interpolar con más exactitud
		Vector3 newPos;
		//calculamos la direccion en la que debemos interpolar
		Vector3 direction = _serverDirection*Vector3(1,0,1);
					//calculamos el movimiento que debe hacer el monigote, mucho mas lento del que debe hacer de normal
		//direction *=(_entity->getComponent<CAvatarController>("CAvatarController")->getVelocity()/5);
		direction*=(_entity->getComponent<CAvatarController>("CAvatarController")->getVelocity()*Vector3(1,0,1)).length()*0.3;
		//si nos hemos pasado, debemos moverlo al sitio
		if(direction.length() > _distance){
			direction*=(_distance/direction.length());
		}
		//std::cout << direction.length() << " contra " << _entity->getComponent<CAvatarController>("CAvatarController")->getVelocity().length() << std::endl;
		//_entity->getComponent<CAvatarController>("CAvatarController")->addForce(Vector3(direction));
		_entity->getComponent<CPhysicController>("CPhysicController")->move(direction,msecs);
		_distance -= direction.length();
		//std::cout << "nueva pos lenght " << _distance << std::endl ;
		/*if(_canInterpolateRotation){

			//si la diferencia es demasiado grande, lo movemos a pelo
			if(_yawDifference > _maxYaw || _yawDifference < _maxYaw*(-1)){
				Matrix3 a;
				_serverPos.extract3x3Matrix(a);
				_entity->setOrientation(a);
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
		}//if(_canInterpolateRotation)*/

		//si hemos terminado de interpolar, lo dejamos
		if((_distance < _minDistance)/* && ( _yawDifference < _minYaw || _yawDifference > _minYaw*(-1) )*/){
			_interpolating = false;
			std::cout << "interpolate a false " << _distance << " ---- " << _minDistance << std::endl << std::endl;
		}
		
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::process(const std::shared_ptr<CMessage>& message) {
		switch(message->getMessageType())
		{
		case Message::SYNC_POSITION:
			{
			std::shared_ptr<CMessageSyncPosition> syncMsg = std::static_pointer_cast<CMessageSyncPosition>(message);

			// nos guardamos la posi que nos han dado por si tenemos que interpolar
			_serverPos = syncMsg->getTransform();
			//calculo el ping que tengo ahora mismo	
			int ping = clock()+Logic::CServer::getSingletonPtr()->getDiffTime()-syncMsg->getTime();
			_actualPing = abs(ping);
			//calculamos la interpolacion
			calculateInterpolation();
			break;
			}
		}
	} // process

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::calculateInterpolation(){
		
		//primero debemos setear la posición del servidor, suponiendo donde debe estar en el tiempo
		//que ha tardado la información en llegar hasta aquí y utilizando la velocidad a la que
		//nos estamos moviendo actualmente
		Vector3 serverPos = _serverPos.getTrans();

		if((serverPos-_entity->getPosition()).length()< _minDistance)
			return;
		Vector3 serverDisplacement = _entity->getComponent<CAvatarController>("CAvatarController")->getVelocity();

		//esta es la posi que suponemos que tiene el server en eeste momento
		if(_actualPing > _msecs)
			serverDisplacement*=_actualPing/_msecs;
		else
			serverPos+=serverDisplacement;

		//direccion de interpolación
		Vector3 intDirection = (serverPos - _entity->getPosition())*Vector3(1,0,1);

		float distance = intDirection.length();

		//si nuestra distancia es inadmisible, lo ponemos donde nos ha dicho el servidor mas lo que hemos supuesto
		if(distance > _maxDistance){
			_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(serverPos);
			//Movemos la orientacion logica/grafica
			Matrix3 tmpMatrix;
			_serverPos.extract3x3Matrix(tmpMatrix);
			_entity->setOrientation(tmpMatrix);
			return;
		}
		//si la distancia es mayor que min distance y menor que maxDistance interpolamos
		else if(distance > _minDistance){
			//nos guardamos la dirección en la que tenemos que interpolar
			_serverDirection = intDirection.normalisedCopy();
			_distance = distance;

			_interpolating = true;

		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::activate(){
		IComponent::activate();
		_serverDirection = Vector3(0,0,0);
		_interpolating = false;
		_distance = 0;
	}

} // namespace Logic

