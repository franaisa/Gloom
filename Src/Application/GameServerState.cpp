//---------------------------------------------------------------------------
// GameServerState.cpp
//---------------------------------------------------------------------------

/**
@file GameServerState.cpp

Contiene la implementación del estado de juego.

@see Application::CGameServerState
@see Application::CGameState

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "GameServerState.h"

#include "Net/Manager.h"
#include "Net/Servidor.h"
#include "Net/factoriared.h"
#include "Net/paquete.h"
#include "Net/buffer.h"

#include "Logic/GameNetPlayersManager.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityID.h"
#include "Input\PlayerController.h"
#include "Input/Server.h"

namespace Application {


	void CGameServerState::activate() {
		CGameState::activate();

		_netMgr = Net::CManager::getSingletonPtr();

		// Añadir a este estado como observador de la red para ser notificado
		_netMgr->addObserver(this);

		//nos registramos como observadores del teclado
		Input::CInputManager::getSingletonPtr()->addKeyListener(this);

		// Seteamos el máximo de jugadores a 12 (8 players + 4 espectadores)
		_netMgr->activateAsServer(1234,12);

		_playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
		
	} // activate

	//______________________________________________________________________________

	void CGameServerState::deactivate() {
		CGameState::deactivate();
		
		_playersMgr = NULL;
		_netMgr = NULL;

		// Solicitamos dejar de ser notificados
		_netMgr->removeObserver(this);
		// Nos desconectamos
		_netMgr->deactivateNetwork();

		Input::CInputManager::getSingletonPtr()->removeKeyListener(this);

		
	} // deactivate

	//______________________________________________________________________________
	
	void CGameServerState::dataPacketReceived(Net::CPaquete* packet) {
		// Obtenemos la id de la conexion por la que hemos recibido el paquete (para identificar
		// al cliente)
		Net::NetID playerNetId = packet->getConexion()->getId();

		// Construimos un buffer para leer los datos que hemos recibido
		Net::CBuffer inBuffer( packet->getDataLength() );
		inBuffer.write( packet->getData(), packet->getDataLength() );
		inBuffer.reset(); // Desplazamos el puntero al principio para realizar la lectura

		// En primer lugar extraemos el tipo del mensaje que hemos recibido
		Net::NetMessageType msgType;
		inBuffer.read( &msgType, sizeof(msgType) );
		
		switch(msgType) {
			case Net::PLAYER_INFO: {
				// Deserializamos el nombre del player y el mesh (en un futuro la clase del player)
				std::string playerNick;
				inBuffer.deserialize(playerNick);

				// Registramos al player en el gestor de jugadores
				_playersMgr->addPlayer(playerNetId, playerNick);

				// Una vez recibida la informacion del cliente, le indicamos que cargue el mapa
				
				// Obtenemos el nombre del mapa que el servidor tiene en uso
				std::string map = Logic::CServer::getSingletonPtr()->getMap()->getMapName();
				map = map.substr( 0, map.find("_") );

				// Construimos el mensaje de load_map con el nombre del mapa
				Net::NetMessageType loadMapMsg = Net::LOAD_MAP;
				Net::CBuffer outBuffer( sizeof(loadMapMsg) + sizeof(unsigned int) + (map.size() * sizeof(char)) );
				outBuffer.write( &loadMapMsg, sizeof(loadMapMsg) );
				outBuffer.serialize(map, false);
				
				// Enviamos el mensaje de carga de mapa al cliente
				_netMgr->sendTo(playerNetId, outBuffer.getbuffer(), outBuffer.getSize());

				break;
			}
			case Net::MAP_LOADED: {
				// Una vez cargado el mapa, comienza la fase de carga de players.
				// El player que esta siendo conectado deberá cargar a todos los players que ya estaban online
				// y vicerversa, los players que ya estabán online deberán cargar al nuevo.

				// Variables locales
				Net::NetMessageType loadPlayersMsg = Net::LOAD_PLAYERS;
				Logic::TEntityID entityId;
				std::string name, playerClass;
				int nbPlayersSpawned = _playersMgr->getNumberOfPlayersSpawned();
				
				// Mandamos la información asociada a los players que ya están conectados al player que desea
				// conectarse
				Logic::CPlayerInfo tempPlayerInfo;
				Net::CBuffer playersInfoBuffer;
				// Escribimos la cabecera
				playersInfoBuffer.write(&loadPlayersMsg, sizeof(loadPlayersMsg));
				// Escribimos el numero de players que hay online
				playersInfoBuffer.write(&nbPlayersSpawned, sizeof(nbPlayersSpawned));

				Logic::CGameNetPlayersManager::iterator it = _playersMgr->begin();
				for(; it != _playersMgr->end(); ++it) {
					tempPlayerInfo = *it; // Informacion asociada al player
					
					// Solo mandamos la informacion de aquellos players que ya estan dentro
					// de la partida
					if( tempPlayerInfo.isSpawned() ) {
						entityId = tempPlayerInfo.getEntityId().first; // id logico
						name = tempPlayerInfo.getName(); // nickname
						playerClass = tempPlayerInfo.getPlayerClass(); // playerclass

						// Escribimos los datos asociados a este player
						playersInfoBuffer.write( &entityId, sizeof(entityId) );
						playersInfoBuffer.serialize(name, false);
						playersInfoBuffer.serialize(playerClass, false);
					}
				}
				
				// Enviamos los datos asociados a los players online al nuevo player
				_netMgr->sendTo(playerNetId, playersInfoBuffer.getbuffer(), playersInfoBuffer.getSize());

				break;
			}
			case Net::PLAYERS_LOADED: {
				Net::NetMessageType loadWorldStateMsg = Net::LOAD_WORLD_STATE;
				// De momento no serializamos nada, pero aqui habria que liarla
				// parda para mandarle todo lo que toque al cliente
				_netMgr->sendTo(playerNetId, &loadWorldStateMsg, sizeof(loadWorldStateMsg));

				break;
			}
			case Net::WORLD_STATE_LOADED: {
				Net::NetMessageType startGameMsg = Net::START_GAME;
				_netMgr->sendTo(playerNetId, &startGameMsg, sizeof(startGameMsg));
			}
			case Net::PING: {
				Net::NetMessageType ackMsg = Net::PING;
				clock_t time = clock();
				Net::NetID id = _netMgr->getID();
				Net::CBuffer ackBuffer(sizeof(ackMsg) + sizeof(time));
				ackBuffer.write(&ackMsg, sizeof(ackMsg));
				ackBuffer.write(&time, sizeof(time));
				_netMgr->sendTo(playerNetId, ackBuffer.getbuffer(), ackBuffer.getSize());
				break;
			}
			case Net::SPECTATE_REQUEST: {
				// Obtenemos el nickname del jugador que quiere espectar
				std::string nickname = _playersMgr->getPlayerNickname(playerNetId);
				// Creamos la entidad espectador con el nombre del jugador
				Logic::CEntity* spectator = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(nickname, "Spectator");
				// Obtenemos la id logica de la entidad espectador
				Logic::TEntityID spectatorId = spectator->getEntityID();
				// Seteamos la id logica del jugador en el gestor de jugadores
				_playersMgr->setEntityID(playerNetId, spectatorId);

				// Enviamos el mensaje de carga de espectador
				Net::NetMessageType msgType = Net::LOAD_LOCAL_SPECTATOR;
				Net::CBuffer buffer;
				buffer.write( &msgType, sizeof(msgType) );
				buffer.write( &spectatorId, sizeof(spectatorId) );
				buffer.serialize(nickname, false);
				_netMgr->sendTo( playerNetId, buffer.getbuffer(), buffer.getSize() );

				// Activamos al espectador
				spectator->activate();
				spectator->start();
				//_playersMgr->setPlayerState(playerNetId, true);
				break;
			}
			case Net::CLASS_SELECTED: {
				//primero comprobamos si habia una entidad correspondiente a este jugador
				//en caso de que la haya la eliminamos para crear la nueva
				Logic::CEntity* deletePlayer = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(
					_playersMgr->getPlayer(playerNetId).getEntityId().first);
				if(deletePlayer){
					Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(deletePlayer,true);
				}

				int race;
				inBuffer.deserialize(race);

				std::string name = _playersMgr->getPlayerNickname(playerNetId);

				//creamos el jugador que el cliente ha elegido
				Logic::CEntity* player;
				switch(race) {
					case 1:
						player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, "Screamer");
						break;
					case 2:
						player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, "Hound");
						break;
					case 3:
						player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, "Archangel");
						break;
					case 4:
						player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, "Shadow");
						break;
				}

				_playersMgr->setEntityID( playerNetId, player->getEntityID() );
				
				Net::NetMessageType netMsg = Net::LOAD_PLAYERS;// Escribimos el tipo de mensaje de red a enviar
				int nbPlayers = 1;
				Logic::TEntityID playerId = player->getEntityID();
				
				//serializamos toda la información que se necesita para la creación de la entidad
				Net::CBuffer serialMsg;
				serialMsg.write(&netMsg, sizeof(netMsg));
				serialMsg.write(&nbPlayers, sizeof(nbPlayers));
				serialMsg.write(&playerId, sizeof(playerId));
				serialMsg.serialize(player->getName(), false); // Nombre del player
				serialMsg.serialize(player->getType(), false); // Clase del player
				
				//enviamos la entidad nueva al jugador local
				_netMgr->broadcastIgnoring(playerNetId, serialMsg.getbuffer(), serialMsg.getSize());

				serialMsg.reset();
				//enviamos la entidad nueva al resto de jugadores
				netMsg = Net::LOAD_LOCAL_PLAYER;// Escribimos el tipo de mensaje de red a enviar
				//serializamos toda la información que se necesita para la creación de la entidad
				serialMsg.write(&netMsg, sizeof(netMsg));
				serialMsg.serialize(player->getEntityID());
				serialMsg.serialize(player->getName(), false); // Nombre del player
				serialMsg.serialize(player->getType(), false); // Clase del player

				player->activate();
				player->start();

				_netMgr->sendTo(playerNetId, serialMsg.getbuffer(), serialMsg.getSize());

				_playersMgr->setPlayerState(playerNetId, true);
				
				break;
			}
		}

	} // dataPacketReceived

	//______________________________________________________________________________

	void CGameServerState::connectionPacketReceived(Net::CPaquete* packet) {
		Net::NetID playerId = packet->getConexion()->getId();
		
		// Actualizamos el manager de jugadores
		//_playersMgr->addPlayer(playerId);

		// Avisamos al player que quiere conectarse de que nos envie la información
		// asociada a su player
		Net::NetMessageType msg = Net::SEND_PLAYER_INFO;

		_netMgr->sendTo(playerId, &msg, sizeof(msg));
	} // connexionPacketReceived

	//______________________________________________________________________________

	void CGameServerState::disconnectionPacketReceived(Net::CPaquete* packet) {
		// Obtenemos el puntero al gestor de jugadores y el id de red del cliente que se quiere desconectar
		Net::NetID playerNetId = packet->getConexion()->getId();
		// Eliminamos la entidad (este mensaje se forwardea automaticamente a los clientes).
		Logic::CEntity* entityToBeDeleted = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID( _playersMgr->getPlayerId(playerNetId) );
		Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(entityToBeDeleted,true);
		// Eliminamos el jugador que se desconecta del manager de jugadores
		_playersMgr->removePlayer(playerNetId);
	} // disconnexionPacketReceived

	bool CGameServerState::keyPressed(Input::TKey key)
	{
		return CGameState::keyPressed(key);;

	} // keyPressed

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CGameServerState::keyReleased(Input::TKey key)
	{

		return CGameState::keyReleased(key);

	} // keyReleased

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CGameServerState::mouseMoved(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------

	bool CGameServerState::mousePressed(const Input::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameServerState::mouseReleased(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
	
} // namespace Application
