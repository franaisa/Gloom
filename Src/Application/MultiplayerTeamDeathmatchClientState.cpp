//---------------------------------------------------------------------------
// MultiplayerTeamDeathmatchClientState.cpp
//---------------------------------------------------------------------------

/**
@file MultiplayerTeamDeathmatchClientState.cpp

Contiene la implementación del estado de juego.

@see Application::MultiplayerTeamDeathmatchClientState
@see Application::CGameState

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "MultiplayerTeamDeathmatchClientState.h"
#include "OgreClock.h"
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

namespace Application {

	
	bool CMultiplayerTeamDeathmatchClientState::init(){

		
		
		
		return true;
	}
	//______________________________________________________________________________

	void CMultiplayerTeamDeathmatchClientState::activate() {
		CGameState::activate();

		// Registramos a este estado como observador de red para que sea notificado
		Net::CManager::getSingletonPtr()->addObserver(this);
		
		//comenzamos el proceso de sincronizacion, para ello enviamos un mensaje de ping
		//y tomamos el tiempo para cronometrar cuanto tarda el servidor en respondernos
		Net::NetMessageType ackMsg = Net::PING;
		Net::NetID id = Net::CManager::getSingletonPtr()->getID();
		Net::CBuffer ackBuffer(sizeof(ackMsg) + sizeof(id));
		ackBuffer.write(&ackMsg, sizeof(ackMsg));
		ackBuffer.write(&id, sizeof(id));
		_reloj = clock();
		_npings = 0;
		_pingActual = 0;
		Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize());
	} // activate

	//______________________________________________________________________________

	void CMultiplayerTeamDeathmatchClientState::deactivate() {
		// Indicamos que ya no queremos ser notificados por la red
		Net::CManager::getSingletonPtr()->removeObserver(this);
		// Nos desconectamos
		Net::CManager::getSingletonPtr()->deactivateNetwork();

		CGameState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	void CMultiplayerTeamDeathmatchClientState::dataPacketReceived(Net::CPaquete* packet) {
		Net::CBuffer buffer(packet->getDataLength());
		buffer.write(packet->getData(), packet->getDataLength());
		buffer.reset();

		Net::NetMessageType msg;
		buffer.read(&msg, sizeof(msg));

		switch (msg) {
			case Net::LOAD_PLAYER: {
				// Creamos el player. Deberíamos poder propocionar caracteríasticas
				// diferentes según el cliente (nombre, modelo, etc.). Esto es una
				// aproximación, solo cambiamos el nombre y decimos si es el jugador
				// local. Los datos deberían llegar en el paquete de red.
				Net::NetID id;
				Logic::TEntityID entityID;
				//memcpy(&id, packet->getData() + sizeof(msg), sizeof(id));
				buffer.read(&id, sizeof(id));
				buffer.read(&entityID, sizeof(entityID));
				std::string name;
				buffer.deserialize(name);

				Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, entityID);
				player->activate();
				break;
			}
			case Net::PING: {
				// me llega la respuesta de un ping, por lo tanto tomo el tiempo y calculo mi ping
				unsigned int time = clock();
				unsigned int ping = time - _reloj;
				_npings++;
				_pingActual += ping/2;

				if(_npings>5) { //si ya he tomado suficientes pings, calculo la media y la seteo al server
					_pingActual = _pingActual/_npings;
					clock_t serverTime;
					buffer.read(&serverTime,sizeof(serverTime));
					serverTime+=_pingActual;
					Logic::CServer::getSingletonPtr()->setDiffTime(serverTime-time);
				}
				else { //si no he tomado suficientes pings, me guardo el ping y envío otro
					Net::NetMessageType msgping = Net::PING;
					Net::NetID id = Net::CManager::getSingletonPtr()->getID();
					Net::CBuffer ackBuffer(sizeof(msgping) + sizeof(id));
					ackBuffer.write(&msgping, sizeof(msgping));
					ackBuffer.write(&id, sizeof(id));
					_reloj = clock();
					Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize());
				}

				break;
			}
			case Net::PLAYER_LEFT: {
				// Obtenemos el id del player que ha dejado la partida
				Logic::TEntityID playerLeftId;
				buffer.read( &playerLeftId, sizeof(playerLeftId) );

				// Obtenemos el puntero a la entidad que corresponde con el id dado
				Logic::CEntity* entityToBeDeleted = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(playerLeftId);

				// Borramos la entidad
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(entityToBeDeleted);
			}
		}
	}

};