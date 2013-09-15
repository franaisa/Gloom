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

#include "Logic/GameNetPlayersManager.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/WorldState.h"
#include "Logic/Maps/EntityID.h"
#include "Input/PlayerController.h"
#include "Input/Server.h"

namespace Application {

	void CGameServerState::activate() {
		CGameState::activate();

		// Inicializamos los punteros a las clases mas usadas
		_netMgr = Net::CManager::getSingletonPtr();
		_playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
		_map = Logic::CServer::getSingletonPtr()->getMap();

		// Nos registramos como observadores de la red para ser notificados
		_netMgr->addObserver(this);

		// Seteamos el máximo de jugadores a 12 (8 players + 4 espectadores)
		// @deprecated Deberiamos tomar el valor de flash en lobbyServer y
		// tomar el numero de jugadores que haya en el gestor de jugadores
		_netMgr->activateAsServer(1234, 12);

		// Nos registramos como observadores del teclado
		Input::CInputManager::getSingletonPtr()->addKeyListener(this);

		_worldState = Logic::CWorldState::getSingletonPtr();
	} // activate

	//______________________________________________________________________________

	void CGameServerState::deactivate() {
		Input::CInputManager::getSingletonPtr()->removeKeyListener(this);

		_playersMgr = NULL;
		_netMgr = NULL;
		_map = NULL;
		_worldState = NULL;

		CGameState::deactivate();
	} // deactivate

	//______________________________________________________________________________
	
	void CGameServerState::sendMapInfo(Net::NetID playerNetId) {
		// Una vez recibida la informacion del cliente, le indicamos que cargue el mapa
				
		// Obtenemos el nombre del mapa que el servidor tiene en uso
		std::string mapName = _map->getMapName();
		mapName = mapName.substr( 0, mapName.find("_") );

		// Construimos el mensaje de load_map con el nombre del mapa
		Net::NetMessageType loadMapMsg = Net::LOAD_MAP;
		Net::CBuffer outBuffer( sizeof(loadMapMsg) + sizeof(unsigned int) + (mapName.size() * sizeof(char)) );
		outBuffer.write( &loadMapMsg, sizeof(loadMapMsg) );
		outBuffer.serialize(mapName, false);
				
		// Enviamos el mensaje de carga de mapa al cliente
		_netMgr->sendTo(playerNetId, outBuffer.getbuffer(), outBuffer.getSize());
	}

	//______________________________________________________________________________

	void CGameServerState::sendWorldState(Net::NetID playerNetId) {
		Net::CBuffer worldBuffer = Logic::CWorldState::getSingletonPtr()->serialize();
		Net::CBuffer scoreboardBuffer = _playersMgr->serializeScoreboardInfo();

		Net::CBuffer sendWorldState;
		Net::NetMessageType loadPlayersMsg = Net::LOAD_WORLD_STATE;

		sendWorldState.write( &loadPlayersMsg,sizeof(loadPlayersMsg) );
		sendWorldState.write( worldBuffer.getbuffer(), worldBuffer.getSize() );
		sendWorldState.write( scoreboardBuffer.getbuffer(), scoreboardBuffer.getSize() );
				
		// Enviamos los datos asociados a los players online al nuevo player
		_netMgr->sendTo(playerNetId, sendWorldState.getbuffer(), sendWorldState.getSize());
	}

	//______________________________________________________________________________

	void CGameServerState::createAndMirrorSpectator(Net::NetID playerNetId) {
		// Primero comprobamos si habia una entidad correspondiente a este jugador
		// ya que durante el juego tambien podemos cambiar de clase.
		// En caso de que la haya la eliminamos para crear la nueva
		std::pair<Logic::TEntityID, bool> id = _playersMgr->getPlayerId(playerNetId);
		if(id.second) { // Si el id devuelto es valido
			Logic::CEntity* deletePlayer = _map->getEntityByID( id.first );
			if(deletePlayer) {
				Logic::CEntityFactory::getSingletonPtr()->deleteEntity(deletePlayer, true);
			}
		}
		
		// Obtenemos el nickname del jugador que quiere espectar
		std::string nickname = _playersMgr->getPlayerNickname(playerNetId);
		// Creamos la entidad espectador con el nombre del jugador
		Logic::CEntity* spectator = _map->createPlayer(nickname, "Spectator");
		// Obtenemos la id logica de la entidad espectador
		Logic::TEntityID spectatorId = spectator->getEntityID();
		// Seteamos la id logica del jugador en el gestor de jugadores
		_playersMgr->setEntityID(playerNetId, spectatorId);

		// Escribimos el tipo de mensaje de red a enviar
		Net::NetMessageType netMsg = Net::LOAD_PLAYERS;
		int nbPlayers = 1;
				
		// Serializamos toda la información que se necesita para la creación de la entidad
		Net::CBuffer buffer;
		buffer.write(&netMsg, sizeof(netMsg));
		buffer.write(&nbPlayers, sizeof(nbPlayers));
		buffer.write(&playerNetId, sizeof(playerNetId));
		buffer.write(&spectatorId, sizeof(spectatorId));
		buffer.serialize(nickname, false); // Nombre del player
		buffer.serialize("Spectator", false); // Clase del player
		// Enviamos la entidad nueva al resto de jugadores
		_netMgr->broadcastIgnoring(playerNetId, buffer.getbuffer(), buffer.getSize());

		buffer.reset();

		// Enviamos la entidad nueva al jugador local
		netMsg = Net::LOAD_LOCAL_PLAYER;
		// Serializamos toda la información que se necesita para la creación de la entidad
		buffer.write(&netMsg, sizeof(netMsg));
		buffer.write(&playerNetId, sizeof(playerNetId));
		buffer.write(&spectatorId, sizeof(spectatorId));
		buffer.serialize(nickname, false); // Nombre del player
		buffer.serialize("Spectator", false); // Clase del player

		spectator->activate();
		spectator->start();

		_netMgr->sendTo(playerNetId, buffer.getbuffer(), buffer.getSize());

		_playersMgr->setPlayerState(playerNetId, false);
		_playersMgr->setPlayerTeam(playerNetId, Logic::TeamFaction::eNONE);
	}

	//______________________________________________________________________________

	void CGameServerState::createAndMirrorPlayer(int race, Net::NetID playerNetId, Logic::TeamFaction::Enum team) {
		// Primero comprobamos si habia una entidad correspondiente a este jugador
		// ya que durante el juego tambien podemos cambiar de clase.
		// En caso de que la haya la eliminamos para crear la nueva
		std::pair<Logic::TEntityID, bool> id = _playersMgr->getPlayerId(playerNetId);
		if(id.second) { // Si el id devuelto es valido
			Logic::CEntity* deletePlayer = _map->getEntityByID( id.first );
			if(deletePlayer) {
				Logic::CEntityFactory::getSingletonPtr()->deleteEntity(deletePlayer, true);
			}
		}
		
		std::string name = _playersMgr->getPlayerNickname(playerNetId);
		
		// Obtenemos el nombre de la clase a la que pertenece el player
		std::string playerClass;
		switch(race) {
			case 1:
				playerClass = "Screamer";
				break;
			case 2:
				playerClass = "Hound";
				break;
			case 3:
				playerClass = "Archangel";
				break;
			case 4:
				playerClass = "Shadow";
				break;
		}

		// Creamos el player
		Logic::CEntity* player = _map->createPlayer(name, playerClass);
		// Seteamos la id logica asociada al player
		Logic::TEntityID playerId = player->getEntityID();
		_playersMgr->setEntityID(playerNetId, playerId);

		// Escribimos el tipo de mensaje de red a enviar
		Net::NetMessageType netMsg = Net::LOAD_PLAYERS;
		int nbPlayers = 1;
				
		// Serializamos toda la información que se necesita para la creación de la entidad
		Net::CBuffer buffer;
		buffer.write(&netMsg, sizeof(netMsg));
		buffer.write(&nbPlayers, sizeof(nbPlayers));
		buffer.write(&playerNetId, sizeof(playerNetId));
		buffer.write(&playerId, sizeof(playerId));
		buffer.serialize(player->getName(), false); // Nombre del player
		buffer.serialize(player->getType(), false); // Clase del player
		buffer.write(&team, sizeof(team));
		// Enviamos la entidad nueva al resto de jugadores
		_netMgr->broadcastIgnoring(playerNetId, buffer.getbuffer(), buffer.getSize());

		buffer.reset();

		// Enviamos la entidad nueva al jugador local
		netMsg = Net::LOAD_LOCAL_PLAYER;

		// Serializamos toda la información que se necesita para la creación de la entidad
		buffer.write(&netMsg, sizeof(netMsg));
		buffer.write(&playerNetId, sizeof(playerNetId));
		buffer.write(&playerId, sizeof(playerId));
		buffer.serialize(player->getName(), false); // Nombre del player
		buffer.serialize(player->getType(), false); // Clase del player
		buffer.write(&team, sizeof(team));
		std::cout << "el equipo que estoy enviando es ... " << team << std::endl;
		player->activate();
		player->start();

		_netMgr->sendTo(playerNetId, buffer.getbuffer(), buffer.getSize());

		_playersMgr->setPlayerState(playerNetId, true);
		_playersMgr->setPlayerTeam(playerNetId, team);
	}

	//______________________________________________________________________________

	void CGameServerState::dataPacketReceived(Net::CPaquete* packet) {
		// Obtenemos la id de la conexion por la que hemos recibido 
		// el paquete (para identificar al cliente)
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
				// Deserializamos el nombre del player
				std::string playerNick;
				inBuffer.deserialize(playerNick);
				//comprobamos si hay algún jugador con el mismo nombre en la partida, y si es así,
				//se lo cambiamos para que no haya problemas en el futuro
				if(_map->getEntityByName(playerNick)){
					std::ostringstream convert;
					convert << playerNetId;
					std::string nameId = convert.str();
					playerNick += nameId;
				}
				// Registramos al player en el gestor de jugadores
				_playersMgr->addPlayer(playerNetId, playerNick);
				// Enviamos la informacion de carga de mapa al cliente
				sendMapInfo(playerNetId);

				break;
			}
			case Net::MAP_LOADED: {
				Net::CBuffer gameSettingsBuffer;
				Net::NetMessageType msgType = Net::GAME_SETTINGS;

				gameSettingsBuffer.write(&msgType, sizeof(msgType));
				
				gameSettingsBuffer.write(&_gameMode, sizeof(_gameMode));
				gameSettingsBuffer.write(&_gameTime, sizeof(_gameTime));
				gameSettingsBuffer.write(&_goalScore, sizeof(_goalScore));

				_netMgr->sendTo( playerNetId, gameSettingsBuffer.getbuffer(), gameSettingsBuffer.getSize() );

				break;
			}
			case Net::GAME_SETTINGS_LOADED: {
				// Una vez cargado el mapa, comienza la fase de carga de players.
				// El player que esta siendo conectado deberá cargar a todos los players que ya estaban online.
				sendWorldState(playerNetId);

				break;
			}
			case Net::WORLD_STATE_LOADED: {
				Net::NetMessageType startGameMsg = Net::START_GAME;
				_netMgr->sendTo(playerNetId, &startGameMsg, sizeof(startGameMsg));
				break;
			}
			case Net::CLASS_SELECTED: {
				int race;
				inBuffer.deserialize(race);
				
				if(race == 5) {
					unsigned int nbSpectators = 0;
					for(auto it = _playersMgr->begin(); it != _playersMgr->end(); ++it) {
						std::pair<Logic::TEntityID, bool> id = _playersMgr->getPlayerId(playerNetId);
						if(id.second) {
							if(Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id.first)->getType() == "Spectator")
								++nbSpectators;
						}
					}

					if(nbSpectators < _maxSpectators) {
						// Creamos una entidad espectador y la replicamos en el cliente
						createAndMirrorSpectator(playerNetId);
					}
					else {
						// Mandamos un mensaje de que no existen slots disponibles
						// para jugar
						Net::NetMessageType noSlotsMsg = Net::NO_FREE_SPECTATOR_SLOTS;
						Net::CBuffer errorBuffer( sizeof(noSlotsMsg) );
						errorBuffer.write( &noSlotsMsg, sizeof(noSlotsMsg) );
						_netMgr->sendTo(playerNetId, errorBuffer.getbuffer(), errorBuffer.getSize());
					}
				}
				else {
					// Si quedan slots para conectarse como jugador permitimos que
					// se conecte, sino le mandamos un mensaje
					bool isChanging = false;
					if( _playersMgr->existsByNetId(playerNetId) ) {
						isChanging = _playersMgr->getPlayer(playerNetId).isSpawned();
					}

					// Si estamos cambiando de clase, consideramos un jugador menos, ya que
					// nosotros si no contariamos
					unsigned nbPlayersPlaying = _playersMgr->getNumberOfPlayersSpawned();
					if(isChanging)
						--nbPlayersPlaying;

					if( nbPlayersPlaying < _maxPlayers) {
						Logic::TeamFaction::Enum team;
						if(_autoBalanceTeams) {
							if(_playersMgr->blueTeamPlayers() < _playersMgr->redTeamPlayers()) {
								team = Logic::TeamFaction::eBLUE_TEAM;
							}
							else {
								team = Logic::TeamFaction::eRED_TEAM;
							}
						}
						else {
							if(_gameMode == GameMode::eTEAM_DEATHMATCH ||
							   _gameMode == GameMode::eCAPTURE_THE_FLAG) {

								inBuffer.read(&team, sizeof(team));
							}
							else {
								team = Logic::TeamFaction::eNONE;
							}
						}

						// Creamos una entidad jugador con la clase que nos hayan dicho
						// y la replicamos en el cliente
						createAndMirrorPlayer(race, playerNetId, team);
					}
					else {
						// Mandamos un mensaje de que no existen slots disponibles
						// para jugar
						Net::NetMessageType noSlotsMsg = Net::NO_FREE_PLAYER_SLOTS;
						Net::CBuffer errorBuffer( sizeof(noSlotsMsg) );
						errorBuffer.write( &noSlotsMsg, sizeof(noSlotsMsg) );
						_netMgr->sendTo(playerNetId, errorBuffer.getbuffer(), errorBuffer.getSize());
					}
				}

				break;
			}
			case Net::PING: {
				Net::NetMessageType ackMsg = Net::PING;
				clock_t time = clock();
				Net::CBuffer ackBuffer(sizeof(ackMsg) + sizeof(time));
				ackBuffer.write(&ackMsg, sizeof(ackMsg));
				ackBuffer.write(&time, sizeof(time));
				_netMgr->sendTo(playerNetId, ackBuffer.getbuffer(), ackBuffer.getSize());
				break;
			}
			case Net::LOCAL_PLAYER_LOADED:
			{
				Logic::TEntityID id;
				inBuffer.deserialize(id);
				Logic::CEntity* player = _map->getEntityByID(id);
				player->activate();
				player->start();

				break;
			}
		}

	} // dataPacketReceived

	//______________________________________________________________________________

	void CGameServerState::connectionPacketReceived(Net::CPaquete* packet) {
		Net::NetID playerId = packet->getConexion()->getId();
		Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
		if(playersMgr->getNumberOfPlayersConnected() >= _maxPlayers + _maxSpectators) {
			Net::NetMessageType msgType = Net::MATCH_IS_FULL;
			_netMgr->sendTo( playerId, &msgType, sizeof(msgType) ); 
		}
		else {
			// Avisamos al player que quiere conectarse de que nos envie la información
			// asociada a su player
			Net::NetMessageType msg = Net::SEND_PLAYER_INFO;
			_netMgr->sendTo( playerId, &msg, sizeof(msg) );
		}
	} // connexionPacketReceived

	//______________________________________________________________________________

	void CGameServerState::disconnectionPacketReceived(Net::CPaquete* packet) {
		// Obtenemos el puntero al gestor de jugadores y el id de red del cliente que se quiere desconectar
		Net::NetID playerNetId = packet->getConexion()->getId();

		// Eliminamos la entidad manejada por el cliente que se quiere desconectar.
		// Para ello comprobamos si tiene asignado un id de entidad. Si no lo tiene, es que
		// no ha sido creada ninguna entidad (podria estar conectandose).
		if( _playersMgr->existsByNetId(playerNetId) ) {
			std::pair<Logic::TEntityID, bool> logicIdPair = _playersMgr->getPlayerId(playerNetId);
			if(logicIdPair.second) {
				Logic::CEntity* entityToBeDeleted = _map->getEntityByID(logicIdPair.first);
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(entityToBeDeleted, true);

				// A nivel logico, conviene que los clientes sepan quien se desconecta,
				// especialmente para eliminar cosas del hud
				Net::NetMessageType ackMsg = Net::PLAYER_OFF_MATCH;
				Net::CBuffer disconnectMsg;
				disconnectMsg.write(&ackMsg, sizeof(ackMsg));
				disconnectMsg.write(&playerNetId, sizeof(playerNetId));
				disconnectMsg.serialize(entityToBeDeleted->getName(), false);

				_netMgr->broadcast(disconnectMsg.getbuffer(), disconnectMsg.getSize());
			}

			// Eliminamos el jugador que se desconecta del manager de jugadores
			_playersMgr->removePlayer(playerNetId);
		}
	} // disconnexionPacketReceived

	//______________________________________________________________________________

	void CGameServerState::serverSettings(const std::string& serverName, const std::string& serverPassword, unsigned int maxPlayers, 
								   unsigned int maxSpectators, bool voteMap, bool voteKick) {

		this->_serverName = serverName;
		this->_serverPassword = serverPassword;
		this->_maxPlayers = maxPlayers;
		this->_maxSpectators = maxSpectators;
		this->_voteMap = voteMap;
		this->_voteKick = voteKick;
	}

	//______________________________________________________________________________

	void CGameServerState::disconnect() {
		// Solicitamos dejar de ser notificados
		_netMgr->removeObserver(this);
		// Nos desconectamos
		_netMgr->deactivateNetwork();
	}

	//______________________________________________________________________________

	bool CGameServerState::keyPressed(Input::TKey key) {
		return CGameState::keyPressed(key);
	} // keyPressed

	//______________________________________________________________________________

	bool CGameServerState::keyReleased(Input::TKey key) {
		switch(key.keyId) {
			case Input::Key::ESCAPE: {
				// De momento salimos de la partida, pero esto deberia
				// descubrir un menu de opciones
				disconnect();
				_app->setState("menu");
				break;
			}
			default: {
				return false;
			}
		}
		return true;
	} // keyReleased

	//______________________________________________________________________________

	bool CGameServerState::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//______________________________________________________________________________

	bool CGameServerState::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//______________________________________________________________________________

	bool CGameServerState::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased
	
} // namespace Application
