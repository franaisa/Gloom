//---------------------------------------------------------------------------
// DMServer.cpp
//---------------------------------------------------------------------------

/**
@file DMServer.cpp

@see Application::CDMServer
@see Application::CGameServerState

@author Francisco Aisa García
@date Junio, 2013
*/

#include "DMServer.h"
#include "Logic/Messages/Message.h"
#include <math.h>
#include <vector>

using namespace std;

namespace Application {

	CDMServer::CDMServer(CBaseApplication* app) : CGameServerState(app),
												  _fragLimit(30),
												  _frags(0),
												  _time(900000),
												  _voteKick(false) { 

		// Nada que hacer
	}

	//______________________________________________________________________________

	void CDMServer::setGameConfig(const pair<unsigned int, unsigned int>& timeLimit, unsigned int fragLimit, bool voteKick) {
		// Establecemos el tiempo de partida
		this->_time = (timeLimit.first * 60000) + (timeLimit.second * 1000);
		// Establecemos el limite de frags
		this->_fragLimit = fragLimit;
		// Activamos el votekick
		this->_voteKick = voteKick;
	}

	//______________________________________________________________________________

	void CDMServer::tick(unsigned int msecs) {
		CGameServerState::tick(msecs);

		// Controlamos el tiempo de la partida
		_time -= msecs;
		if(_time < 0) {
			// Fin de partida
			_time = 0;
			// Poner el reloj a 00:00 en el HUD
		}
		else {
			// Emitimos un mensaje para que se actualice el HUD
			// con el formato mm::ss
			unsigned int minutes = ceil( (double)msecs / 60000.0 );
			unsigned int seconds = (msecs % 60000) / 1000;
		}
	}

	//______________________________________________________________________________

	void CDMServer::activate() {
		CGameServerState::activate();

		// Construimos la máscara de eventos/mensajes que nos interesan
		// del worldstate
		vector<Logic::TMessageType> eventsMask;
		eventsMask.reserve(1);

		eventsMask.push_back(Logic::Message::PLAYER_DEAD);
		_worldState->addObserver(this, eventsMask);
	} // activate

	//______________________________________________________________________________

	void CDMServer::deactivate() {
		CGameServerState::deactivate();

		_worldState->removeObserver(this);
	} // deactivate

	//______________________________________________________________________________

	bool CDMServer::keyPressed(Input::TKey key) {
		return CGameServerState::keyPressed(key);
	} // keyPressed

	//______________________________________________________________________________

	bool CDMServer::keyReleased(Input::TKey key) {
		return CGameServerState::keyReleased(key);
	} // keyReleased

	//______________________________________________________________________________

	bool CDMServer::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//______________________________________________________________________________

	bool CDMServer::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//______________________________________________________________________________

	bool CDMServer::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased
	
} // namespace Application
