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
	} // activate

	//______________________________________________________________________________

	void CGameServerState::deactivate() {
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
		Net::CBuffer buffer(packet->getDataLength());
		buffer.write(packet->getData(), packet->getDataLength());
		buffer.reset(); // Desplazamos el puntero al principio para realizar la lectura

		// En primer lugar extraemos el tipo del mensaje que hemos recibido
		Net::NetMessageType msg;
		buffer.read(&msg, sizeof(msg));
		
		switch (msg) {
			case Net::PLAYER_INFO: {
				// Deserializamos el nombre del player y el mesh (en un futuro la clase del player)
				std::string playerNick, playerMesh;
				buffer.deserialize(playerNick);
				buffer.deserialize(playerMesh);

				// Registramos estos datos en el gestor de players
				Logic::CGameNetPlayersManager::getSingletonPtr()->setPlayerNickname(playerNetId, playerNick);
				Logic::CGameNetPlayersManager::getSingletonPtr()->setPlayerMesh(playerNetId, playerMesh);

				// Una vez recibida la informacion del cliente, le indicamos que cargue el mapa (solo
				// a ese cliente concreto)
				std::string map = Logic::CServer::getSingletonPtr()->getMap()->getMapName();
				map = map.substr(0,map.find("_"));
				Net::NetMessageType msg = Net::LOAD_MAP;
				Net::CBuffer buffer(sizeof(msg)+map.size()*sizeof(char));
				buffer.write(&msg, sizeof(msg));
				buffer.serialize(map,false);
				Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize(), playerNetId);

				break;
			}
			case Net::MAP_LOADED: {
				// EN ESTA FASE EL CLIENTE PASA PASA A CARGAR LOS CLIENTES DE LA PARTIDA Y VICEVERSA,
				// LOS CLIENTES DE LA PARTIDA PASAN A CARGAR EL NUEVO CLIENTE

				// Variables locales
				std::string name;
				Logic::TEntityID entityId;
				Net::NetID netId;
				Net::NetMessageType msg = Net::LOAD_PLAYER;

				// Mandamos la informacion de los players de la partida al cliente que esta intentando conectarse
				Logic::CPlayerInfo tempPlayerInfo;
				Net::CBuffer tempBuffer(sizeof(msg)+sizeof(netId)+sizeof(entityId)+sizeof(name));

				Logic::CGameNetPlayersManager::iterator it = Logic::CGameNetPlayersManager::getSingletonPtr()->begin();
				for(; it != Logic::CGameNetPlayersManager::getSingletonPtr()->end(); ++it) {
					tempPlayerInfo = *it;
					netId = tempPlayerInfo.getNetId();

					// Debido a que fuera de este bucle enviaremos la informacion de este player mediante broadcast
					// evitamos enviar la informacion en esta fase (ya que la id de entidad aun no ha sido asignada)
					if(netId != playerNetId) {
						entityId = tempPlayerInfo.getEntityId().first;
						name = tempPlayerInfo.getName();

						// Mensaje LOAD_PLAYER
						tempBuffer.write(&msg, sizeof(msg));
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
				buffer.read(&playerLoadedNetId, sizeof(playerLoadedNetId));
				Logic::CGameNetPlayersManager::getSingletonPtr()->loadPlayer( playerNetId, playerLoadedNetId );

				// MANDAR EL MENSAJE DE ARRANQUE A TAN SOLO UNO DE ELLOS (EL QUE SE QUIERE CONECTAR)

				unsigned int playersLoadedByClient = Logic::CGameNetPlayersManager::getSingletonPtr()->getPlayersLoaded(playerNetId);
				unsigned int playersConnected = Logic::CGameNetPlayersManager::getSingletonPtr()->getNumberOfPlayersConnected();

				//Si todos los clientes han cargado todos los players
				if( Logic::CGameNetPlayersManager::getSingletonPtr()->getPlayersLoaded(playerNetId) == 
					Logic::CGameNetPlayersManager::getSingletonPtr()->getNumberOfPlayersConnected() - 1) {
				
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

				buffer.deserialize(race);

				Logic::CPlayerInfo playerInfo = Logic::CGameNetPlayersManager::getSingletonPtr()->getPlayer(playerNetId);
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
				Logic::CGameNetPlayersManager::getSingletonPtr()->setEntityID(playerNetId, player->getEntityID());
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
		Logic::CGameNetPlayersManager::getSingletonPtr()->addPlayer(playerId);

		// Avisamos (solo) al player de que comience a enviarnos sus datos
		Net::NetMessageType msg = Net::LOAD_PLAYER_INFO;
		Net::CBuffer buffer(sizeof(msg));
		buffer.write(&msg, sizeof(msg));

		Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize(), playerId);
	} // connexionPacketReceived

	//______________________________________________________________________________

	void CGameServerState::disconnexionPacketReceived(Net::CPaquete* packet) {
		// Obtenemos el puntero al gestor de jugadores y el id de red del cliente que se quiere desconectar
		Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
		Net::NetID playerNetId = packet->getConexion()->getId();

		// Eliminamos la entidad (este mensaje se forwardea automaticamente a los clientes).
		Logic::CEntity* entityToBeDeleted = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID( playersMgr->getPlayerId(playerNetId) );
		Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(entityToBeDeleted);

		// Eliminamos el jugador que se desconecta del manager de jugadores
		playersMgr->removePlayer(playerNetId);
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
