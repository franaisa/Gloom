/**
@file SpikeTramp.cpp

Contiene la implementación del componente que controla el movimiento de los pinchos.
 
@see Logic::CSpikeTramp
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Mayo, 2013
*/

#include "SpikeTramp.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageTouched.h"


namespace Logic 
{
	IMP_FACTORY(CSpikeTramp);
	
	//---------------------------------------------------------
	
	bool CSpikeTramp::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		//Cogemos los pinchos asociados a esta trampas
		CEntity* pincho1 = CServer::getSingletonPtr()->getMap()->getEntityByName("Pincho11");
		CEntity* pincho2 = CServer::getSingletonPtr()->getMap()->getEntityByName("Pincho12");
		CEntity* pincho3 = CServer::getSingletonPtr()->getMap()->getEntityByName("Pincho13");

		if(entityInfo->hasAttribute("coolDown"))
			_coolDownTime = entityInfo->getFloatAttribute("coolDown");

		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CSpikeTramp::onActivate()
	{

	} // activate
	//---------------------------------------------------------


	bool CSpikeTramp::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CSpikeTramp::process(const std::shared_ptr<CMessage>& message) {
		
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


