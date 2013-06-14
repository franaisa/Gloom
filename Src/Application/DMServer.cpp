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

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/GameNetPlayersManager.h"

#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/Message.h"

#include <math.h>
#include <vector>
#include <string>

using namespace std;

namespace Application {

	CDMServer::CDMServer(CBaseApplication* app) : CGameServerState(app),
												  _fragLimit(1),
												  _time(900000),
												  _voteKick(false),
												  _voteMap(false),
												  _inEndGame(false) {

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

	void CDMServer::gameEventOcurred(Logic::CEntity* emitter, const shared_ptr<Logic::CMessage>& msg) {
		switch( msg->getMessageType() ) {
			case Logic::Message::PLAYER_DEAD: {
				shared_ptr<Logic::CMessagePlayerDead> playerDeadMsg = static_pointer_cast<Logic::CMessagePlayerDead>(msg);
				
				Logic::TEntityID killerID = playerDeadMsg->getKiller();
				Logic::TEntityID emitterID = emitter->getEntityID();
				Logic::CEntity* killer = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(killerID);
				if( emitter != killer && isPlayer(killer) ) {
					_playersMgr->addFragUsingEntityID(killerID);

					if(_playersMgr->getFragsUsingEntityID(killerID) == _fragLimit) {
						// fin de partida
						endGame();
					}
				}
				else {
					_playersMgr->substractFragUsingEntityID(emitterID);
				}

				cout << killer->getName() << " lleva " << _playersMgr->getFragsUsingEntityID(emitterID) << " frags" << endl;

				break;
			}
		}
	}

	//______________________________________________________________________________

	void CDMServer::tick(unsigned int msecs) {
		// Si la partida no ha acabado, actualizamos la física y la lógica
		// y actualizamos el reloj de partida
		if(!_inEndGame) {
			CGameServerState::tick(msecs);

			// Controlamos el tiempo de la partida
			_time -= msecs;
			if(_time < 0) {
				endGame();
			}
		}
		// Si la partida ha finalizado dejamos de hacer tick
		else {
			_time -=msecs;
			if(_time < 0) {
				_time = 0;

				if(_autoChangeMap) {
					// Pasar al siguiente mapa
					// de la lista
				}
				else {
					// Salir de la partida y desconexion
					//disconnect();
					//_app->setState("menu");
				}
			}
		}
		/*else {
			// @todo En el cliente emitimos un mensaje para que se actualice el HUD
			// con el formato mm::ss
			//unsigned int minutes = _time / 60000;
			//unsigned int seconds = (_time % 60000) / 1000;
		}*/
	}

	//______________________________________________________________________________

	void CDMServer::activate() {
		CGameServerState::activate();

		// Construimos la máscara de eventos/mensajes que nos interesan
		// del worldstate
		vector<Logic::TMessageType> eventsMask;
		//eventsMask.reserve(1);

		eventsMask.push_back(Logic::Message::PLAYER_DEAD);
		_worldState->addObserver(this, eventsMask);
	} // activate

	//______________________________________________________________________________

	void CDMServer::deactivate() {
		_worldState->removeObserver(this);

		CGameServerState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	void CDMServer::endGame() {
		cout << "LA PARTIDA HA ACABADO!" << endl;
		_inEndGame = true;
		// Tiempo de espera hasta la siguiente partida
		_time = _voteMap ? 45000 : 15000;
		// Notificar a los clientes de que estamos en la fase endGame
		Net::NetMessageType endGameMsg = Net::END_GAME;
		_netMgr->broadcast(&endGameMsg, sizeof(endGameMsg));

		// Parar la partida (el server y el cliente ya no hacen tick)
		// Poner la cámara mirando al jugador y que al girar el ratón rote alrededor
		// del player - cliente
		// Poner la cámara de los demás jugadores mirando al ganador - cliente
		// Mostrar el scoreboard durante unos 10 segundos - cliente
		// Si votemap está activo mostrar el menú de votemap durante unos 30 segs
		// Si votemap no está activo entonces pasar al siguiente mapa de los configurados
		// y si no ha habido ningun mapa puesto en lista salir
	}

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

	//______________________________________________________________________________

	bool CDMServer::isPlayer(Logic::CEntity* entity) {
		string type = entity->getType();
		
		return type == "Screamer"		|| 
			   type == "Shadow"			|| 
			   type == "Hound"			|| 
			   type == "Archangel"		||
			   type == "LocalScreamer"	|| 
			   type == "LocalShadow"	|| 
			   type == "LocalHound"		|| 
			   type == "LocalArchangel";
	}
	
} // namespace Application
