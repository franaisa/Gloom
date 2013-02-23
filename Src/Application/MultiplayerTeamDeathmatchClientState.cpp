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

	void CMultiplayerTeamDeathmatchClientState::activate() {
		CGameState::activate();

		// Registramos a este estado como observador de red para que sea notificado
		Net::CManager::getSingletonPtr()->addObserver(this);
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

		if(msg == Net::LOAD_PLAYER) {
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

			//Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, entityID);
			
			// NO ES NECESARIO ENVIAR LOAD_PLAYER EN EL CASO DE LOS QUE JUEGAN

			//Enviamos el mensaje de que se ha creado el jugador
			/*Net::NetMessageType ackMsg = Net::PLAYER_LOADED;
			Net::CBuffer ackBuffer(sizeof(ackMsg) + sizeof(id));
			ackBuffer.write(&ackMsg, sizeof(ackMsg));
			ackBuffer.write(&id, sizeof(id));
			Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize());*/
		}
	}

};