/**
@file Spike.cpp

Contiene la implementación del componente que controla los contactos con el pincho.
 
@see Logic::CSpike
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero,Mayo, 2013
*/

#include "Spike.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicDynamicEntity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageDeactivate.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"

namespace Logic 
{
	IMP_FACTORY(CSpike);
	
	//---------------------------------------------------------
	
	bool CSpike::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_initialPosition = entityInfo->getVector3Attribute("position");

		return true;

	} // spawn
	//---------------------------------------------------------

	bool CSpike::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CSpike::process(const std::shared_ptr<CMessage>& message) {
		
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				std::cout << "El pincho toca" << std::endl;
				//Aplicamos el daño
				std::shared_ptr<CMessageTouched> touchedMsg = std::static_pointer_cast<CMessageTouched>(message);
				std::shared_ptr<CMessageDamaged> damagedMsg = std::make_shared<CMessageDamaged>();
				damagedMsg->setDamage(1000);
				damagedMsg->setEnemy(_entity);
				touchedMsg->getEntity()->emitMessage(damagedMsg);
				//Desactivamos la fuerza que lleva el pincho
				_entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity")->clearForce(Physics::ForceMode::eFORCE);
				//Desactivamos grafica y fisicamente
				//std::shared_ptr<CMessageDeactivate> deactivateMsg = std::make_shared<CMessageDeactivate>();
				//_entity->emitMessage(deactivateMsg);
				//Recolocamos la entidad fisicamente en su inicio para cuando se active nuevamente la trampa
				std::shared_ptr<CMessageSetPhysicPosition> setPhysicPositionMsg = std::make_shared<CMessageSetPhysicPosition>();
				setPhysicPositionMsg->setPosition(_initialPosition);
				_entity->emitMessage(setPhysicPositionMsg);
				break;
			}
		}

	} // process
	//----------------------------------------------------------


} // namespace Logic


