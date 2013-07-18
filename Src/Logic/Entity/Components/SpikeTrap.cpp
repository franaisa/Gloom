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
#include "Logic/Maps/EntityFactory.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageAudio.h"

using namespace std;

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
		_velocitySpikes = entityInfo->getIntAttribute("velocitySpikes");
		_directionSpikes = entityInfo->getVector3Attribute("directionSpikes");
		_audioTrap = entityInfo->getStringAttribute("audioTrap");

		//Lectura de los pinchos asociados a la trampa
		for(int i=1; i<=_numSpikes; i++){
			std::stringstream numI;
			numI << i;
			std::string nI = numI.str();
			_spikesPosition.push_back(entityInfo->getVector3Attribute("spike"+nI+"Position"));
			_spikesYaw.push_back(entityInfo->getFloatAttribute("spike"+nI+"Yaw"));
			_spikesPitch.push_back(entityInfo->getFloatAttribute("spike"+nI+"Pitch"));		
		}

	} // spawn
	
	//---------------------------------------------------------
	
	void CSpikeTrap::onActivate()
	{
		_isRespawning=false;
		_timer=0;
	} // activate
	//---------------------------------------------------------

	bool CSpikeTrap::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CSpikeTrap::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				//Creamos los pinchos
				std::vector<CEntity*> spikes=createSpikes();
				for(auto it = spikes.begin(); it != spikes.end(); ++it) {
					//Creamos el mensaje de fuerza para los pinchos
					std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::make_shared<CMessageAddForcePhysics>();
					forceMsg->setForce(_directionSpikes* _velocitySpikes, Physics::ForceMode::eFORCE );
					forceMsg->setGravity(false);
					(*it)->emitMessage(forceMsg);
					
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
				}
				
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
	
	//OJO SE PODRIA HACER QUE EN DICHO METODO AL SPIKE SE LE ASIGNE DE OWNER EL QUE ACTIVO LA TRAMPA ASI SERA COMO MATAR CON DICHA TRAMPA Y SUMARA FRAGS
	std::vector<CEntity*> CSpikeTrap::createSpikes(){
		// Vector de pinchos a devolver
		std::vector<CEntity*> spikes;
		for(int i=0;i<_numSpikes;i++){
			// Obtenemos la información estandard asociada a un pincho
			Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Spike")->clone();

			// Creamos la entidad
			CEntity* spike= CEntityFactory::getSingletonPtr()->createEntity(
									entityInfo,
									CServer::getSingletonPtr()->getMap(),
									_spikesPosition[i],
									Math::setQuaternion(Math::fromDegreesToRadians(_spikesYaw[i]),Math::fromDegreesToRadians(_spikesPitch[i]),0),
									true
								);

			// Eliminamos la copia de la información que hemos creado
			delete entityInfo;

			// Arrancamos la entidad
			spike->activate();
			spike->start();
		
			spikes.push_back(spike);
		}

		return spikes;

	} // createSpikes
	//----------------------------------------------------------
	

} // namespace Logic


