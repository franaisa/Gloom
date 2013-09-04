//---------------------------------------------------------------------------
// TDMClient.cpp
//---------------------------------------------------------------------------

/**
@file TDMClient.cpp

@see Application::CDMClient
@see Application::CGameClientState

@author Francisco Aisa García
@date Junio, 2013
*/

#include "TDMClient.h"
#include "Input/Server.h"
#include "Input/PlayerController.h"
#include "Input/InputManager.h"
#include "Logic/Maps/Scoreboard.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Hud.h"
#include "Net/paquete.h"

using namespace std;

namespace Application {

	void CTDMClient::tick(unsigned int msecs) {
		CGameClientState::tick(msecs);

		if(_infiniteTime) return;

		// Controlamos el tiempo de la partida
		_gameTime -= msecs;
		if(_gameTime < 0) {
			// Fin de partida
			_gameTime = 0;

			// En el cliente
			// Mostramos el tiempo en el hud del player
			Logic::CEntity* player = Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar();
			// De momento solo mostramos el tiempo si el player esta dentro de la partida (en los menus no se 
			// muestra el tiempo).
			if(player != NULL) {
				Logic::CHud* hudComponent = player->getComponent<Logic::CHud>("CHud");
				// De momento el espectador no tiene hud, por eso hago esta
				// comprobacion
				if(hudComponent != NULL)
					hudComponent->updateMatchTime(0, 0);
			}
		}
		else {
			// @todo En el cliente emitimos un mensaje para que se actualice el HUD
			// con el formato mm::ss
			unsigned int minutes = _gameTime / 60000;
			unsigned int seconds = (_gameTime % 60000) / 1000;

			if(this->_minutes != minutes || this->_seconds != seconds) {
				this->_minutes = minutes;
				this->_seconds = seconds;

				// Mostramos el tiempo en el hud del player
				Logic::CEntity* player = Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar();
				// De momento solo mostramos el tiempo si el player esta dentro de la partida (en los menus no se 
				// muestra el tiempo).
				if(player != NULL) {
					Logic::CHud* hudComponent = player->getComponent<Logic::CHud>("CHud");
					// De momento el espectador no tiene hud, por eso hago esta
					// comprobacion
					if(hudComponent != NULL)
						hudComponent->updateMatchTime(minutes, seconds);
				}
			}
		}
	}

	//______________________________________________________________________________

	void CTDMClient::activate() {
		CGameClientState::activate();
		_inEndGame = false;
	} // activate

	//______________________________________________________________________________

	void CTDMClient::deactivate() {
		CGameClientState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	void CTDMClient::dataPacketReceived(Net::CPaquete* packet) {
		CGameClientState::dataPacketReceived(packet);

		// Introducimos los datos recibidos en un buffer
		Net::CBuffer buffer( packet->getDataLength() );
		buffer.write( packet->getData(), packet->getDataLength() );
		buffer.reset();

		// Obtenemos la cabecera del mensaje
		Net::NetMessageType netMsgType;
		buffer.read( &netMsgType, sizeof(netMsgType) );

		// Dependiendo del tipo de mensaje
		switch (netMsgType) {
			case Net::END_GAME: {
				_inEndGame = true;
				
				// Desactivamos al jugador
				Logic::CEntity* player = Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar();
				set<string> exceptionList;
				exceptionList.insert("CAnimationManager");

				player->deactivateAllComponentsExcept(exceptionList);

				// Fijamos la cámara en el exterior para que rote a nuestro
				// alrededor
				Logic::CScoreboard::getSingletonPtr()->activate();

				break;
			}
		}
	}

	//______________________________________________________________________________

	bool CTDMClient::keyPressed(Input::TKey key) {
		CGameClientState::keyPressed(key);

		switch(key.keyId) {
			case Input::Key::TAB: {
				if(!_inEndGame)
					Logic::CScoreboard::getSingletonPtr()->activate();
				
				break;
			}
		}
		
		return true;
	} // keyPressed

	//______________________________________________________________________________

	bool CTDMClient::keyReleased(Input::TKey key) {
		CGameClientState::keyReleased(key);

		switch(key.keyId) {
			case Input::Key::TAB: {
				if(!_inEndGame)
					Logic::CScoreboard::getSingletonPtr()->deactivate();
				
				break;
			}
		}

		return true;
	} // keyReleased

	//______________________________________________________________________________

	bool CTDMClient::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//______________________________________________________________________________

	bool CTDMClient::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//______________________________________________________________________________

	bool CTDMClient::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased
	
} // namespace Application
