//---------------------------------------------------------------------------
// DMClient.cpp
//---------------------------------------------------------------------------

/**
@file DMClient.cpp

@see Application::CDMClient
@see Application::CGameClientState

@author Francisco Aisa García
@date Junio, 2013
*/

#include "DMClient.h"
#include "Input/Server.h"
#include "Input/PlayerController.h"
#include "Input/InputManager.h"
#include "Logic/Maps/Scoreboard.h"
#include "Net/paquete.h"

namespace Application {

	void CDMClient::tick(unsigned int msecs) {
		CGameClientState::tick(msecs);

		// Controlamos el tiempo de la partida
		_time -= msecs;
		if(_time < 0) {
			// Fin de partida
			_time = 0;

			// En el cliente
			// Poner el reloj a 00:00 en el HUD
		}
		else {
			// @todo En el cliente emitimos un mensaje para que se actualice el HUD
			// con el formato mm::ss
			//unsigned int minutes = _time / 60000;
			//unsigned int seconds = (_time % 60000) / 1000;
		}
	}

	//______________________________________________________________________________

	void CDMClient::activate() {
		CGameClientState::activate();
		_inEndGame = false;
	} // activate

	//______________________________________________________________________________

	void CDMClient::deactivate() {
		CGameClientState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	void CDMClient::dataPacketReceived(Net::CPaquete* packet) {
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
				Input::CServer::getSingletonPtr()->getPlayerController()->deactivate();
				// Fijamos la cámara en el exterior para que rote a nuestro
				// alrededor
				Logic::CScoreboard::getSingletonPtr()->activate();

				break;
			}
		}
	}

	//______________________________________________________________________________

	bool CDMClient::keyPressed(Input::TKey key) {
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

	bool CDMClient::keyReleased(Input::TKey key) {
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

	bool CDMClient::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//______________________________________________________________________________

	bool CDMClient::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//______________________________________________________________________________

	bool CDMClient::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased
	
} // namespace Application
