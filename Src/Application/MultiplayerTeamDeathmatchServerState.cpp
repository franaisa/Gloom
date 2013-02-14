//---------------------------------------------------------------------------
// MultiplayerTeamDeathmatchServerState.cpp
//---------------------------------------------------------------------------

/**
@file MultiplayerTeamDeathmatchServerState.cpp

Contiene la implementación del estado de juego.

@see Application::MultiplayerTeamDeathmatchServerState
@see Application::CGameState

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "MultiplayerTeamDeathmatchServerState.h"

#include "Net/Manager.h"
#include "Net/Servidor.h"
#include "Net/factoriared.h"
#include "Net/paquete.h"
#include "Net/buffer.h"

#include "Logic/GameNetPlayersManager.h"

namespace Application {


	void CMultiplayerTeamDeathmatchServerState::activate() 
	{
		CGameState::activate();

		Net::CManager::getSingletonPtr()->addObserver(this);
	} // activate

	//--------------------------------------------------------

	void CMultiplayerTeamDeathmatchServerState::deactivate() 
	{
		Net::CManager::getSingletonPtr()->removeObserver(this);
		Net::CManager::getSingletonPtr()->deactivateNetwork();

		CGameState::deactivate();
	} // deactivate

	//--------------------------------------------------------
	
	void CMultiplayerTeamDeathmatchServerState::dataPacketReceived(Net::CPaquete* packet)
	{
		// Obtenemos la id de la conexion por la que hemos recibido el paquete (para identificar
		// al cliente)
		Net::NetID playerId = packet->getConexion()->getId();

		Net::CBuffer buffer(packet->getDataLength());
		buffer.write(packet->getData(), packet->getDataLength());
		buffer.reset(); // Desplazamos el puntero al principio para realizar la lectura

		Net::NetMessageType msg;
		buffer.read(&msg, sizeof(msg));
		
		switch (msg)
		{
		case Net::PLAYER_INFO:
		{
			std::string playerNick, playerMesh;
			buffer.deserialize(playerNick);
			buffer.deserialize(playerMesh);

			Logic::CGameNetPlayersManager::getSingletonPtr()->setPlayerNickname(playerId, playerNick);
			Logic::CGameNetPlayersManager::getSingletonPtr()->setPlayerMesh(playerId, playerMesh);

			// Una vez recibida la informacion del cliente, le indicamos que cargue el mapa (solo
			// a ese cliente concreto)
			Net::NetMessageType msg = Net::LOAD_MAP;
			Net::CBuffer buffer(sizeof(msg));
			buffer.write(&msg, sizeof(msg));

			Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize(), playerId);

			break;
		}
		case Net::MAP_LOADED:
		{
			// EN ESTA FASE EL CLIENTE PASA PASA A CARGAR LOS CLIENTES DE LA PARTIDA Y VICEVERSA,
			// LOS CLIENTES DE LA PARTIDA PASAN A CARGAR EL NUEVO CLIENTE

			// Se debe crear un jugador por cada cliente registrado.
			/*for(TNetIDList::const_iterator it = _clients.begin(); it != _clients.end(); it++) {
				//Preparamos la lista de control de carga de jugadores.
				//Esto quiere decir que el cliente (*it) ha cargado 0 jugadores
				TNetIDCounterPair elem(*it,0);
				_playersLoadedByClients.insert(elem);

				// @todo Hay que enviar un paquete tipo LOAD_PLAYER con 
				// el NetID del cliente del que estamos creando el jugador (*it)
				Net::NetMessageType msg = Net::LOAD_PLAYER;
				Net::NetID netId = *it;
					
				Net::CBuffer buffer(sizeof(msg) + sizeof(netId) + sizeof(Logic::EntityID));
				buffer.write(&msg, sizeof(msg));
				buffer.write(&netId, sizeof(netId));

				// Mandar el ID del cliente actual a todos los clientes para que
				// lo carguen
					
				// Creamos el player. Deberíamos poder propocionar caracteríasticas
				// diferentes según el cliente (nombre, modelo, etc.). Esto es una
				// aproximación, solo cambiamos el nombre y decimos si es el jugador
				// local
				std::string name("Player");
				std::stringstream number;
				number << (*it);
				name.append(number.str());

				// @todo Llamar al método de creación del jugador. Deberemos decidir
				// si el jugador es el jugador local. Al ser el servidor ninguno lo es

				Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name);

				Logic::TEntityID id = player->getEntityID();
				buffer.write(&id, sizeof(id));

				Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize());
			}
			
			break;
		}
		case Net::PLAYER_LOADED:
		{
			//Aumentamos el número de jugadores cargados por el cliente
			(*_playersLoadedByClients.find(packet->getConexion()->getId())).second++;

			// @todo Comprobar si todos los clientes han terminado de 
			// cargar todos los jugadores
			bool loadFinished = true;
			TNetIDCounterMap::const_iterator it2;
			for(TNetIDList::const_iterator it = _clients.begin(); it != _clients.end() && loadFinished; it++) {
				// Obtener la pareja (NetID, jugadores cargados)
				it2 = _playersLoadedByClients.find(*it);
				
				if(it2 != _playersLoadedByClients.end()) {
					// Si el numero de jugadores cargados para este cliente es menor
					// que el numero total de clientes cargar, entonces es que aun
					// hay clientes que no han cargado a todos los jugadores
					if(it2->second < _clients.size()) {
						loadFinished = false;
					}
				}
			}
			//Si todos los clientes han cargado todos los players
			if(loadFinished)
			{
				//Enviamos el mensaje de que empieza el juego a todos los clientes
				Net::NetMessageType msg = Net::START_GAME;
				Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg));
				_app->setState("multiplayerTeamDeathmatchServer");
			}
			break;*/
		}
		}

	} // dataPacketReceived

	//--------------------------------------------------------

	void CMultiplayerTeamDeathmatchServerState::connexionPacketReceived(Net::CPaquete* packet)
	{
		Net::NetID playerId = packet->getConexion()->getId();

		// @TODO
		// Garantizar que todos los clientes se conectan en la fase adecuada de manera que no se
		// produzcan condiciones indeseables a causa de la concurrencia
		
		// Actualizamos el manager de jugadores
		Logic::CGameNetPlayersManager::getSingletonPtr()->addPlayer(playerId);

		// Avisamos (solo) al player de que comience a enviarnos sus datos
		Net::NetMessageType msg = Net::LOAD_PLAYER_INFO;
		Net::CBuffer buffer(sizeof(msg));
		buffer.write(&msg, sizeof(msg));

		Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize(), playerId);
	} // connexionPacketReceived

	//--------------------------------------------------------

	void CMultiplayerTeamDeathmatchServerState::disconnexionPacketReceived(Net::CPaquete* packet)
	{
		// Actualizamos el manager de jugadores
		Logic::CGameNetPlayersManager::getSingletonPtr()->removePlayer( packet->getConexion()->getId() );

		// @todo
		// NOTIFICAR A LOS CLIENTES PARA QUE ELIMINEN ESE JUGADOR DE LA PARTIDA
	} // disconnexionPacketReceived
	
} // namespace Application
