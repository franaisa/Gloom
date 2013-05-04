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

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddForcePhysics.h"


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

		_numTrap = entityInfo->getIntAttribute("numTrap");
		_numSpikes = entityInfo->getIntAttribute("numSpikes");
		_spikes = new CEntity*[_numSpikes];
		_velocitySpikes= entityInfo->getIntAttribute("velocitySpikes");
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
				//LLamamos a la activacion de los pinchos asociados y los lanzamos
				std::cout << "ACTIVAMOS LA TRAMPA" << std::endl;
				//Dirección en la que irán los pinchos
				Vector3 directionSpikes=Math::getDirection(_spikes[0]->getOrientation());
				directionSpikes.normalise();
				//Creamos el mensaje de fuerza para los pinchos
				std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::make_shared<CMessageAddForcePhysics>();
				forceMsg->setForce(directionSpikes* _velocitySpikes, Physics::ForceMode::eFORCE );
				forceMsg->setGravity(false);
				//Lo suyo seria activarlos para que se vean y se pueda contactar con ellos fisicamente
				//Y luego darles el mensaje de fuerza ( ambos en el bucle de abajo )
				for(int i=0; i<_numSpikes; i++){
					_spikes[i]->emitMessage(forceMsg);
				}
				break;
			}
		}

	} // process
	//----------------------------------------------------------


} // namespace Logic


