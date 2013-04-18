/**
@file ClientRespawn.cpp

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "ClientRespawn.h"

#include "Logic/Entity/Entity.h"
#include "PhysicController.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"
#include "Interpolation.h"

#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/MessagePlayerSpawn.h"

#include <math.h>

#include <iostream>

namespace Logic  {
	
	IMP_FACTORY(CClientRespawn);

	//________________________________________________________________________

	bool CClientRespawn::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::PLAYER_DEAD ||
			   message->getMessageType() == Message::PLAYER_SPAWN;
		//return false;
	} // accept

	//________________________________________________________________________

	void CClientRespawn::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PLAYER_DEAD: {
				// El servidor nos notifica de que hemos muerto, desactivamos
				// la entidad al completo y su simulacion fisica para que no
				// podamos colisionar con la cápsula del jugador.

				// Desactivamos todos los componentes menos estos
				std::vector<std::string> exceptionList;
				exceptionList.push_back( std::string("CClientRespawn") );
				exceptionList.push_back( std::string("CAnimatedGraphics") );
				exceptionList.push_back( std::string("CHudOverlay") );
				exceptionList.push_back( std::string("CNetConnector") );

				// En caso de estar simulando fisica en el cliente, desactivamos
				// la cápsula.
				CPhysicController* controllerComponent = _entity->getComponent<CPhysicController>("CPhysicController");
				if(controllerComponent != NULL) {
					controllerComponent->deactivateSimulation();
				}

				_entity->deactivateAllComponentsExcept(exceptionList);
			
				break;
			}
			case Message::PLAYER_SPAWN: {
				// El servidor nos notifica de que debemos respawnear. Activamos
				// todos los componentes de la entidad y seteamos nuestra posicion
				// en el lugar indicado por el mensaje recibido del servidor.

				std::shared_ptr<CMessagePlayerSpawn> playerSpawnMsg = std::static_pointer_cast<CMessagePlayerSpawn>(message);

				Matrix4 spawnTransform = playerSpawnMsg->getSpawnTransform();

				// En caso de estar simulando fisica en el cliente, reactivamos las colisiones
				// y reposicionamos la capsula donde nos diga el servidor.
				CPhysicController* controllerComponent = _entity->getComponent<CPhysicController>("CPhysicController");
				if(controllerComponent != NULL) {
					// Reactivamos la simulacion
					controllerComponent->activateSimulation();
					// Colocamos al player en la posicion dada por el manager de spawn del server
					controllerComponent->setPhysicPosition( spawnTransform.getTrans() );

					// Seteamos la orientacion a la dada por el server
					Matrix3 spawnOrientation;
					spawnTransform.extract3x3Matrix( spawnOrientation );
					_entity->setOrientation(spawnOrientation);
				}
			
				// Volvemos a activar todos los componentes
				_entity->activate();

				std::shared_ptr<CMessagePlayerSpawn> cameraPlayerSpawnMsg = std::make_shared<CMessagePlayerSpawn>();
				CServer::getSingletonPtr()->getMap()->getEntityByType("Camera")->emitMessage(cameraPlayerSpawnMsg);

				break;
			}
		}
	} // process

} // namespace Logic

