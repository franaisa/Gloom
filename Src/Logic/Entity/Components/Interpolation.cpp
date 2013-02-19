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

namespace Logic 
{
	IMP_FACTORY(CInterpolation);

	//---------------------------------------------------------
	bool CInterpolation::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// Indicar parametros de interpolacion

		return true;
	} // spawn

	//---------------------------------------------------------
	bool CInterpolation::accept(CMessage *message) {
		return message->getMessageType() == Message::SYNC_POSITION;
	} // accept
	
	//---------------------------------------------------------
	void CInterpolation::process(CMessage *message) {
		switch(message->getMessageType())
		{
		case Message::SYNC_POSITION:
			// Set transform por cojones
			//_entity->setTransform( ((CMessageSyncPosition*)message)->getTransform() );
			Matrix4 transform = ((CMessageSyncPosition*)message)->getTransform();
			//Mensaje para el componente de físicas
			Logic::CMessageSetPhysicPosition *m = new Logic::CMessageSetPhysicPosition();
			m->setPosition(transform.getTrans());
			_entity->emitMessage(m);
			//Movemos la orientacion logica/grafica
			Matrix3 tmpMatrix;
			transform.extract3x3Matrix(tmpMatrix);
			_entity->setOrientation(tmpMatrix);

			break;
		}
	} // process


} // namespace Logic

