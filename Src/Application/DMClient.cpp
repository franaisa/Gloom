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
				// Desactivamos al jugador
				Input::CServer::getSingletonPtr()->getPlayerController()->deactivate();
				// Fijamos la cámara en el exterior para que rote a nuestro
				// alrededor

				break;
			}
		}
	}

	//______________________________________________________________________________

	bool CDMClient::keyPressed(Input::TKey key) {
		return CGameClientState::keyPressed(key);
	} // keyPressed

	//______________________________________________________________________________

	bool CDMClient::keyReleased(Input::TKey key) {
		return CGameClientState::keyReleased(key);
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
