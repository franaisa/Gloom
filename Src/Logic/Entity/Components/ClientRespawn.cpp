/**
@file ClientRespawn.cpp

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "ClientRespawn.h"
#include "AvatarController.h"

#include "Logic/Entity/Entity.h"
#include "PhysicController.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/Scoreboard.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"
#include "Interpolation.h"

#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/MessagePlayerSpawn.h"
#include "Logic/Maps/GUIKillersMessage.h"
#include "../../GameNetPlayersManager.h"
#include "Logic/Messages/MessageHudSpawn.h"

#include <math.h>

#include <iostream>

namespace Logic  {
	
	IMP_FACTORY(CClientRespawn);

	//________________________________________________________________________

	bool CClientRespawn::accept(const std::shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::PLAYER_DEAD		||
			   msgType == Message::PLAYER_SPAWN;
		//return false;
	} // accept

	//________________________________________________________________________

	void CClientRespawn::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PLAYER_DEAD:{
				// El servidor nos notifica de que hemos muerto, desactivamos
				// la entidad al completo y su simulacion fisica para que no
				// podamos colisionar con la cápsula del jugador.

				// Desactivamos todos los componentes menos estos
				std::set<std::string> exceptionList;
				exceptionList.insert( std::string("CClientRespawn") );
				exceptionList.insert( std::string("CAnimatedGraphics") );
				exceptionList.insert( std::string("CHudOverlay") );
				exceptionList.insert( std::string("CNetConnector") );
				exceptionList.insert( std::string("CAudio") );
				
				// En caso de estar simulando fisica en el cliente, desactivamos
				// la cápsula.
				_entity->getComponent<CPhysicController>("CPhysicController")->deactivateSimulation();

				CAvatarController* component = _entity->getComponent<CAvatarController>("CAvatarController");
				if(component != NULL) {
					exceptionList.insert( std::string("CAvatarController") );
					_entity->getComponent<CAvatarController>("CAvatarController")->putToSleep(true);
				}

				_entity->deactivateAllComponentsExcept(exceptionList);

				//mostramos en pantalla el mensaje de quien ha matado a quien
				std::shared_ptr<CMessagePlayerDead> playerDeadMsg = std::static_pointer_cast<CMessagePlayerDead> (message);
				CEntity* entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(playerDeadMsg->getKiller());
			
				if(!entity)
					break;

				updateGUI(entity);

				break;
			}
			case Message::PLAYER_SPAWN: {
				// El servidor nos notifica de que debemos respawnear. Activamos
				// todos los componentes de la entidad y seteamos nuestra posicion
				// en el lugar indicado por el mensaje recibido del servidor.

				std::shared_ptr<CMessagePlayerSpawn> playerSpawnMsg = std::static_pointer_cast<CMessagePlayerSpawn>(message);
				// Colocamos al player en la posicion dada por el manager de spawn del server,
				// podemos asumir sin problemas que el player tiene capsula
				
				_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition( playerSpawnMsg->getSpawnPosition() );
				// Activamos la simulacion aqui sin problemas. El componente life ignora los mensajes de daño
				// hasta que no desaparece la inmunidad del respawn. 
				_entity->getComponent<CPhysicController>("CPhysicController")->activateSimulation();

				CAvatarController* component = _entity->getComponent<CAvatarController>("CAvatarController");
				if(component != NULL) {
					component->wakeUp();
				}

				// Seteamos la orientacion a la dada por el server
				_entity->setYaw(playerSpawnMsg->getSpawnOrientation(),true);


				// Volvemos a activar todos los componentes
				_entity->activate();

				Vector3 entityPos = _entity->getPosition();

				std::shared_ptr<CMessagePlayerSpawn> cameraPlayerSpawnMsg = std::make_shared<CMessagePlayerSpawn>();
				CEntity* cam = CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
				assert(cam != NULL && "Error: Esto no se puede hacer asi que sois unos lamers, ahora el servidor que hace?");
				cam->emitMessage(cameraPlayerSpawnMsg);

				std::shared_ptr<CMessageHudSpawn> messageHudSpawn = std::make_shared<CMessageHudSpawn>();
				messageHudSpawn->setTime(0);
				_entity->emitMessage(messageHudSpawn);

				break;
			}
		}
	} // process

	void CClientRespawn::updateGUI(CEntity* killer){
		//updateamos el mensaje de pepito mato a juanito
		std::string type = killer->getType();
			
		if(type == "Screamer" || type == "Shadow" || type == "Hound" || type == "Archangel" ||
			type == "LocalScreamer" || type == "LocalShadow" || type == "LocalHound" || type == "LocalArchangel"){
			Logic::GUIKillersMessage::getSingletonPtr()->addKiller(
				killer->getName(),
				_entity->getName());

			Logic::CScoreboard::getSingletonPtr()->addKill(killer->getName());
			Logic::CScoreboard::getSingletonPtr()->addDeath(_entity->getName());
		}
		//sino ha sido un player es que se ha suicidado el retard
		else{
			Logic::GUIKillersMessage::getSingletonPtr()->suicide(_entity->getName());
			Logic::CScoreboard::getSingletonPtr()->addDeath(_entity->getName());
		}
	}


} // namespace Logic

