/**
@file Interpolation.cpp

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "Interpolation.h"

#include "Logic/Entity/Entity.h"
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

		// Indicar parametros de interpolacion
		_interpolating = false;
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

		//calculamos la distancia que hay entre donde estoy y donde me dice el servidor que debería estar
		Vector3 serverPos = _serverPos.getTrans();
		serverPos = serverPos - _entity->getPosition();
		float distance = sqrt(serverPos.x*serverPos.x+serverPos.y*serverPos.y+serverPos.z+serverPos.z);
		
		//si la distancia es muy grande, entonces seteamos la posi del tirón
		//Mensaje para el componente de físicas
		

	}

	//________________________________________________________________________

	void CInterpolation::process(CMessage *message) {
		switch(message->getMessageType())
		{
		case Message::SYNC_POSITION:
			// nos guardamos la posi que nos han dado por si tenemos que interpolar
			_serverPos = ((CMessageSyncPosition*)message)->getTransform();
			Logic::CMessageSetPhysicPosition *m = new Logic::CMessageSetPhysicPosition();
			m->setPosition(_serverPos.getTrans());
			_entity->emitMessage(m);
			//Movemos la orientacion logica/grafica
			Matrix3 tmpMatrix;
			_serverPos.extract3x3Matrix(tmpMatrix);
			_entity->setOrientation(tmpMatrix);
			break;
		}
	} // process

} // namespace Logic

