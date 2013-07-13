/**
@file SpikeTrap.cpp

Contiene la implementación del componente que controla el movimiento de los pinchos.
 
@see Logic::CSpikeTrap
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Mayo, 2013
*/

#include "SpikeTrap.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Logic/Maps/WorldState.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageAudio.h"



namespace Logic 
{
	IMP_FACTORY(CSpikeTrap);
	
	//---------------------------------------------------------
	
	bool CSpikeTrap::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("coolDown"))
			_coolDownTime = entityInfo->getIntAttribute("coolDown");
		_coolDownTime*=1000;

		_numTrap = entityInfo->getIntAttribute("numTrap");
		_numSpikes = entityInfo->getIntAttribute("numSpikes");
		_spikes = new CEntity*[_numSpikes];
		_velocitySpikes = entityInfo->getIntAttribute("velocitySpikes");
		_directionSpikes = entityInfo->getVector3Attribute("directionSpikes");
		_audioTrap = entityInfo->getStringAttribute("audioTrap");
		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CSpikeTrap::onActivate()
	{
		// Leer los parametros que toquen para los proyectiles
		std::stringstream numTrap;
		numTrap << _numTrap;
		std::string nTrap = numTrap.str();
		//Cogemos los pinchos asociados a esta trampa 
		for(int i=1; i<=_numSpikes; i++){
			std::stringstream numI;
			numI << i;
			std::string nI = numI.str();
			_spikes[i-1] = CServer::getSingletonPtr()->getMap()->getEntityByName("Pincho"+nTrap+nI);
		}
		_isRespawning=false;
		_timer=0;

	} // activate
	//---------------------------------------------------------

	void CSpikeTrap::onStart(){
		//Desactivamos los pinchos para que solo salgan al darle a la trampa
		std::shared_ptr<CMessageActivate> deactivateMsg = std::make_shared<CMessageActivate>();
		deactivateMsg->setActivated(false);
		for(int i=0; i<_numSpikes; i++){
			_spikes[i]->emitMessage(deactivateMsg);
			Logic::CWorldState::getSingletonPtr()->addChange(_spikes[i], deactivateMsg);
		}
	}// onStart
	//---------------------------------------------------------


	bool CSpikeTrap::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CSpikeTrap::process(const std::shared_ptr<CMessage>& message) {
		
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				//Creamos el mensaje de fuerza para los pinchos
				std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::make_shared<CMessageAddForcePhysics>();
				forceMsg->setForce(_directionSpikes* _velocitySpikes, Physics::ForceMode::eFORCE );
				forceMsg->setGravity(false);
				//Activacion y fuerza
				std::shared_ptr<CMessageActivate> activateMsg = std::make_shared<CMessageActivate>();
				for(int i=0; i<_numSpikes; i++){
					activateMsg->setActivated(true);
					_spikes[i]->emitMessage(activateMsg);
					Logic::CWorldState::getSingletonPtr()->addChange(_spikes[i],activateMsg);
					_spikes[i]->emitMessage(forceMsg);
				}
				//Audio
				std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
				audioMsg->setAudioName(_audioTrap);
				audioMsg->isLoopable(false);
				audioMsg->is3dSound(true);
				audioMsg->streamSound(false);
				audioMsg->stopSound(false);
				_entity->emitMessage(audioMsg);

				//Volvemos invisible la trampa
				//Desactivamos la entidad grafica y fisica.
				std::shared_ptr<CMessageActivate> deactivateMsg = std::make_shared<CMessageActivate>();
				deactivateMsg->setActivated(false);
				_entity->emitMessage(deactivateMsg);
				Logic::CWorldState::getSingletonPtr()->addChange(_entity, deactivateMsg);
					
				_isRespawning=true;
				break;
			}
		}

	} // process
	//----------------------------------------------------------


	void CSpikeTrap::onTick(unsigned int msecs){
		// Si la trampa esta en la fase de spawn
		if(_isRespawning) {
			_timer += msecs;
			if(_timer >= _coolDownTime) {
				_isRespawning = false;
				_timer = 0;
				//Volvemos visible la trampa
				//Activamos la entidad grafica y fisica
				std::shared_ptr<CMessageActivate> activateMsg = std::make_shared<CMessageActivate>();
				activateMsg->setActivated(true);
				_entity->emitMessage(activateMsg);
				Logic::CWorldState::getSingletonPtr()->addChange(_entity,activateMsg);
				
			}
		}
	} // onTick
	//----------------------------------------------------------

} // namespace Logic


