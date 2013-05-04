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

		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CSpikeTrap::onActivate()
	{
		//Cogemos los pinchos asociados a esta trampa
		for(int i=0; i<_numSpikes; i++){
			_spikes[i] = CServer::getSingletonPtr()->getMap()->getEntityByName("Pincho"+_numTrap+i);
			_spikes[i] = CServer::getSingletonPtr()->getMap()->getEntityByName("Pincho"+_numTrap+i);
			_spikes[i] = CServer::getSingletonPtr()->getMap()->getEntityByName("Pincho"+_numTrap+i);
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
				break;
			}
		}

	} // process
	//----------------------------------------------------------


} // namespace Logic


