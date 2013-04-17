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

		// Añadir a este estado como observador de la red para ser notificado
		Net::CManager::getSingletonPtr()->addObserver(this);
		Net::CManager::getSingletonPtr()->activateAsServer(1234,16);

		_playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
	} // activate

	//______________________________________________________________________________

	void CGameServerState::deactivate() {
		_playersMgr = NULL;

		// Solicitamos dejar de ser notificados
		Net::CManager::getSingletonPtr()->removeObserver(this);
		// Nos desconectamos
		Net::CManager::getSingletonPtr()->deactivateNetwork();

		CGameState::deactivate();
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
				Net::CManager::getSingletonPtr()->send(outBuffer.getbuffer(), outBuffer.getSize(), playerNetId);

				break;
			}
			case Net::MAP_LOADED: {
				// Una vez cargado el mapa, comienza la fase de carga de players.
				// El player que esta siendo conectado deberá cargar a todos los players que ya estaban online
				// y vicerversa, los players que ya estabán online deberán cargar al nuevo.

				// Variables locales
				std::string name;
				Logic::TEntityID entityId;
				Net::NetID netId;
				Net::NetMessageType loadPlayerMsg = Net::LOAD_PLAYER;

				// Mandamos la informacion de los players de la partida al cliente que esta intentando conectarse
				Logic::CPlayerInfo tempPlayerInfo;
				Net::CBuffer tempBuffer( sizeof(loadPlayerMsg) +sizeof(netId) + sizeof(entityId) + sizeof(name) );

				Logic::CGameNetPlayersManager::iterator it = _playersMgr->begin();
				for(; it != _playersMgr->end(); ++it) {
					tempPlayerInfo = *it;
					netId = tempPlayerInfo.getNetId();

					// Debido a que fuera de este bucle enviaremos la informacion de este player mediante broadcast
					// evitamos enviar la informacion en esta fase (ya que la id de entidad aun no ha sido asignada)
					if(netId != playerNetId) {
						entityId = tempPlayerInfo.getEntityId().first;
						name = tempPlayerInfo.getName();

						// Mensaje LOAD_PLAYER
						tempBuffer.write(&loadPlayerMsg, sizeof(loadPlayerMsg));
						tempBuffer.write(&netId, sizeof(netId));
						tempBuffer.write(&entityId, sizeof(entityId));
						tempBuffer.serialize(name, false);

						Net::CManager::getSingletonPtr()->send(tempBuffer.getbuffer(), tempBuffer.getSize(), playerNetId);

						// Reseteamos el puntero de escritura del buffer para escribir en la siguiente vuelta del bucle
						tempBuffer.reset();
					}
				}

				break;
			}
			case Net::PLAYER_LOADED: {
				//Aumentamos el número de jugadores cargados por el cliente
				Net::NetID playerLoadedNetId;
				inBuffer.read(&playerLoadedNetId, sizeof(playerLoadedNetId));
				_playersMgr->loadPlayer( playerNetId, playerLoadedNetId );

				// MANDAR EL MENSAJE DE ARRANQUE A TAN SOLO UNO DE ELLOS (EL QUE SE QUIERE CONECTAR)

				unsigned int playersLoadedByClient = _playersMgr->getPlayersLoaded(playerNetId);
				unsigned int playersConnected = _playersMgr->getNumberOfPlayersConnected();

				//Si todos los clientes han cargado todos los players
				if( _playersMgr->getPlayersLoaded(playerNetId) == _playersMgr->getNumberOfPlayersConnected() - 1) {
				
					// Si el cliente que queria conectarse a cargado a todos los players mandamos el mensaje
					// de empezar
					Net::NetMessageType msg = Net::START_GAME;
					Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg), playerNetId);
				}

				break;
			}
			case Net::PING: {
				Net::NetMessageType ackMsg = Net::PING;
				clock_t time = clock();
				Net::NetID id = Net::CManager::getSingletonPtr()->getID();
				Net::CBuffer ackBuffer(sizeof(ackMsg) + sizeof(time));
				ackBuffer.write(&ackMsg, sizeof(ackMsg));
				ackBuffer.write(&time, sizeof(time));
				Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize(), playerNetId);
				break;
			}
			case Net::CLASS_SELECTED: {
				//primero comprobamos si habia una entidad correspondiente a este jugador
				//en caso de que la haya la eliminamos para crear la nueva
				Logic::CEntity* deletePlayer = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(
					Logic::CGameNetPlayersManager::getSingletonPtr()->getPlayer(playerNetId).getEntityId().first);
				if(deletePlayer){
					Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(deletePlayer);
				}

				int race;
				std::string name;

				inBuffer.deserialize(race);

				Logic::CPlayerInfo playerInfo = _playersMgr->getPlayer(playerNetId);
				name = playerInfo.getName();
				std::stringstream number;
				number << playerNetId;
				name.append(number.str());

				//creamos el jugador que el cliente ha elegido
				Logic::CEntity* player;
				switch(race)
				{
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
				_playersMgr->setEntityID(playerNetId, player->getEntityID());
				Net::NetMessageType msgType = Net::LOAD_PLAYER;// Escribimos el tipo de mensaje de red a enviar
				Net::CBuffer serialMsg;
				//serializamos toda la información que se necesita para la creación de la entidad
				serialMsg.write(&msgType, sizeof(msgType));
				serialMsg.serialize(player->getEntityID());
				serialMsg.serialize(player->getType(), false);
				serialMsg.serialize(player->getName(), false);
		
				//enviamos la entidad nueva al jugador local
				Net::CManager::getSingletonPtr()->sendAllExcept(serialMsg.getbuffer(), serialMsg.getSize(),playerNetId);
				serialMsg.reset();
				//enviamos la entidad nueva al resto de jugadores
				msgType = Net::LOAD_LOCAL_PLAYER;// Escribimos el tipo de mensaje de red a enviar
				//serializamos toda la información que se necesita para la creación de la entidad
				serialMsg.write(&msgType, sizeof(msgType));
				serialMsg.serialize(player->getEntityID());
				serialMsg.serialize(player->getType(), false);
				serialMsg.serialize(player->getName(), false);
				player->activate();
				Net::CManager::getSingletonPtr()->send(serialMsg.getbuffer(), serialMsg.getSize(),playerNetId);
				break;
				}
			
		}

	} // dataPacketReceived

	//______________________________________________________________________________

	void CGameServerState::connexionPacketReceived(Net::CPaquete* packet) {
		Net::NetID playerId = packet->getConexion()->getId();
		
		// Actualizamos el manager de jugadores
		//_playersMgr->addPlayer(playerId);

		// Avisamos al player que quiere conectarse de que nos envie la información
		// asociada a su player
		Net::NetMessageType msg = Net::SEND_PLAYER_INFO;

		Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg), playerId);
	} // connexionPacketReceived

	//______________________________________________________________________________

	void CGameServerState::disconnexionPacketReceived(Net::CPaquete* packet) {
		// Obtenemos el puntero al gestor de jugadores y el id de red del cliente que se quiere desconectar
		Net::NetID playerNetId = packet->getConexion()->getId();

		// Eliminamos la entidad (este mensaje se forwardea automaticamente a los clientes).
		Logic::CEntity* entityToBeDeleted = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID( _playersMgr->getPlayerId(playerNetId) );
		Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(entityToBeDeleted);

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
