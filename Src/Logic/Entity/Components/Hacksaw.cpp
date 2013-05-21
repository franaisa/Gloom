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


	
	void CHacksaw::onActivate()
	{
		_directionInitial=(_positionInitial-_positionFinal);
		_directionFinal=(_positionFinal-_positionInitial);
		_directionInitial.normalise();
		_directionFinal.normalise();
		_toFinal=true;

	} // activate
	//---------------------------------------------------------

	bool CHacksaw::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CHacksaw::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				std::shared_ptr<CMessageTouched> touchedMsg = std::static_pointer_cast<CMessageTouched>(message);

				std::shared_ptr<Logic::CMessageDamaged> damagedMsg = std::make_shared<Logic::CMessageDamaged>();
				damagedMsg->setDamage(1000);
				damagedMsg->setEnemy(_entity);
				touchedMsg->getEntity()->emitMessage(damagedMsg);
				
				break;
			}
		}

	} // process
	//----------------------------------------------------------

	void CHacksaw::onTick(unsigned int msecs)
	{
		Vector3 toDirection;
		
		//Hacia la posicion final
		if(_toFinal){
			float distanciaToFinal=_positionFinal.squaredDistance(_entity->getPosition());
			//std::cout << "distancia para el final" << distanciaToFinal << std::endl;
			if(distanciaToFinal>=0){
				toDirection = _directionFinal * msecs * _velocity;
				//Por si nos pasasemos de la posición final
				if((_entity->getPosition()+toDirection).squaredDistance(_positionFinal)>distanciaToFinal){
					toDirection=(_positionFinal-_entity->getPosition());
					_toFinal=false;
				}
				
				std::shared_ptr<CMessageKinematicMove> m = std::make_shared<CMessageKinematicMove>();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}
		//Hacia la posicion inicial
		else{
			float distanciaToInitial=_positionInitial.squaredDistance(_entity->getPosition());
			if(distanciaToInitial>=0){
				toDirection = _directionInitial* msecs * _velocity;
				//Por si nos pasasemos de la posición inicial
				if((_entity->getPosition()+toDirection).squaredDistance(_positionInitial)>distanciaToInitial){
					toDirection=(_positionInitial-_entity->getPosition());
					_toFinal=true;
				}

				std::shared_ptr<CMessageKinematicMove> m = std::make_shared<CMessageKinematicMove>();
				m->setMovement(toDirection);
				_entity->emitMessage(m);
			}
		}


	} // tick
	//----------------------------------------------------------


} // namespace Logic


