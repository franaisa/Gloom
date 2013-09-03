//---------------------------------------------------------------------------
// TDMServer.cpp
//---------------------------------------------------------------------------

/**
@file TDMServer.cpp

@see Application::CTDMServer
@see Application::CGameServerState

@author Francisco Aisa García
@date Junio, 2013
*/

#include "TDMServer.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/TeamFaction.h"
#include "Logic/GameNetPlayersManager.h"

#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/Message.h"

#include <math.h>
#include <vector>
#include <string>

using namespace std;

namespace Application {

	CTDMServer::CTDMServer(CBaseApplication* app) : CGameServerState(app, GameMode::eTEAM_DEATHMATCH),
													_forceRespawn(false),
													_warmUp(true),
													_loopMaps(true),
													_currentMap(0),
													_inEndGame(false),
													_unlimitedTime(false),
													_unlimitedScore(false),
													_blueTeamScore(0),
													_redTeamScore(0) {

		// Nada que hacer
	}

	//______________________________________________________________________________

	void CTDMServer::gameSettings(const std::vector<std::string>& mapList, bool loopMaps, const std::pair<unsigned int, unsigned int>& timeLimit, 
								 unsigned int goalScore, bool forceRespawn, bool warmUp, bool autoBalanceTeams) {

		// Establecemos la lista de mapas
		this->_mapList = mapList;
		// Se ejecutan los mapas ciclicamente?
		this->_loopMaps = loopMaps;
		// Establecemos el tiempo de partida
		this->_gameTime = (timeLimit.first * 60000) + (timeLimit.second * 1000);
		// Establecemos el limite de frags
		this->_goalScore = goalScore;
		// Los jugadores respawnean automaticamente?
		this->_forceRespawn = forceRespawn;
		// Existe fase de warmUp?
		this->_warmUp = warmUp;
		// Queremos entrar en un equipo automaticamente?
		this->_autoBalanceTeams = autoBalanceTeams;

		// Comprobamos si se ha configurado el tiempo y el score 
		// como infinito
		if(this->_gameTime == 0)
			_unlimitedTime = true;
		if(this->_goalScore == 0)
			_unlimitedScore = true;
	}

	//______________________________________________________________________________

	void CTDMServer::gameEventOcurred(Logic::CEntity* emitter, const shared_ptr<Logic::CMessage>& msg) {
		switch( msg->getMessageType() ) {
			case Logic::Message::PLAYER_DEAD: {
				shared_ptr<Logic::CMessagePlayerDead> playerDeadMsg = static_pointer_cast<Logic::CMessagePlayerDead>(msg);
				
				Logic::TEntityID killerID = playerDeadMsg->getKiller();
				Logic::TEntityID emitterID = emitter->getEntityID();
				// Si no nos hemos suicidado (ya sea matandonos o muriendo en una trampa)
				Logic::CEntity* killer = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(killerID);
				if( emitter != killer && isPlayer(killer) ) {
					// Incrementamos el número de muertes del jugador
					_playersMgr->addFragUsingEntityID(killerID);
					
					// Obtenemos el equipo al que pertenece el jugador que acaba de hacer un frag
					Logic::TeamFaction::Enum team = _playersMgr->getTeamUsingEntityId(killerID);
					Logic::TeamFaction::Enum victimTeam = _playersMgr->getTeamUsingEntityId(emitterID);
					// Si matas a alguien de tu equipo te descuentas frags
					if(team == victimTeam) {
						if(team == Logic::TeamFaction::eRED_TEAM) {
							--_redTeamScore;
						}
						else if(team == Logic::TeamFaction::eBLUE_TEAM) {
							--_blueTeamScore;
						}
					}
					// Sino incrementas frags y compruebas si has ganado la partida
					else {
						if(team == Logic::TeamFaction::eRED_TEAM) {
							if(++_redTeamScore == _goalScore && !_unlimitedScore) {
								endGame();
								cout << "RED TEAM WINS!" << endl;
							}
						}
						else if(team == Logic::TeamFaction::eBLUE_TEAM) {
							if(++_blueTeamScore == _goalScore && !_unlimitedScore) {
								endGame();
								cout << "BLUE TEAM WINS!" << endl;
							}
						}
					}
				}
				else {
					_playersMgr->substractFragUsingEntityID(emitterID);

					// Obtenemos el equipo al que pertenece el jugador que acaba de suicidarse
					Logic::TeamFaction::Enum team = _playersMgr->getTeamUsingEntityId(killerID);
					if(team == Logic::TeamFaction::eRED_TEAM) {
						--_redTeamScore;
					}
					else if(team == Logic::TeamFaction::eBLUE_TEAM) {
						--_blueTeamScore;
					}
				}

				_playersMgr->addDeathUsingEntityID(emitterID);

				break;
			}
		}
	}

	//______________________________________________________________________________

	void CTDMServer::tick(unsigned int msecs) {
		CGameServerState::tick(msecs);

		// Si la partida ha finalizado comprobamos el tiempo que ha transcurrido
		// Para ver si tenemos que finalizar el encuentro
		if(!_inEndGame && !_unlimitedTime) {
			// Controlamos el tiempo de la partida
			_gameTime -= msecs;
			if(_gameTime < 0) {
				std::cout << "TIME IS OVER!" << std::endl;

				endGame();
			}
		}
		// Si la partida ha finalizado dejamos de hacer tick
		else {
			if(_voteMap) {
				// Mostrar el menu de votacion de mapa
				// En funcion del modo y el mapa que se vote
				// cambiar de mapa o cambiar de modo y mapa
			}
			else {
				_gameTime -= msecs;

				if(_gameTime < 0) {
					if( ++_currentMap < _mapList.size() ) {
						// Cargar el mapa que corresponda a _currentMap
						//changeMap(_mapList[_currentMap]);
					}
					else if(_loopMaps) {
						_currentMap = 0;

						// Cargar el mapa que corresponda a _currentMap
						//changeMap(_mapList[_currentMap]);
					}
					else {
						// Finalizar partida
						disconnect();
						_app->setState("menu");
					}
				}
			}
		}
	}

	//______________________________________________________________________________

	void CTDMServer::activate() {
		CGameServerState::activate();

		// Construimos la máscara de eventos/mensajes que nos interesan
		// del worldstate
		vector<Logic::TMessageType> eventsMask;
		//eventsMask.reserve(1);

		eventsMask.push_back(Logic::Message::PLAYER_DEAD);
		_worldState->addObserver(this, eventsMask);
	} // activate

	//______________________________________________________________________________

	void CTDMServer::deactivate() {
		_worldState->removeObserver(this);

		CGameServerState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	void CTDMServer::endGame() {
		_inEndGame = true;
		// Tiempo de espera hasta la siguiente partida
		_gameTime = _voteMap ? 40000 : 15000;
		// Notificar a los clientes de que estamos en la fase endGame
		Net::NetMessageType endGameMsg = Net::END_GAME;
		_netMgr->broadcast( &endGameMsg, sizeof(endGameMsg) );

		// Desactivamos los componentes relevantes en el servidor para asegurarnos
		// de que aunque el cliente haga chetos no se muevan los jugadores sin permiso
		// ni se hagan daño.
		Logic::CEntity* player;
		std::vector<std::string> componentList;
		componentList.reserve(4);
		componentList.push_back("CAvatarController");
		componentList.push_back("CPhysicController");
		componentList.push_back("CLife");
		componentList.push_back("CSpawnPlayer");

		for(auto it = _playersMgr->begin(); it != _playersMgr->end(); ++it) {
			if( it->getEntityId().second ) {
				player = _map->getEntityByID( it->getEntityId().first );
				player->deactivateComponents(componentList);
			}
		}

		// @todo
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

	bool CTDMServer::keyPressed(Input::TKey key) {
		return CGameServerState::keyPressed(key);
	} // keyPressed

	//______________________________________________________________________________

	bool CTDMServer::keyReleased(Input::TKey key) {
		return CGameServerState::keyReleased(key);
	} // keyReleased

	//______________________________________________________________________________

	bool CTDMServer::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//______________________________________________________________________________

	bool CTDMServer::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//______________________________________________________________________________

	bool CTDMServer::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased

	//______________________________________________________________________________

	bool CTDMServer::isPlayer(Logic::CEntity* entity) {
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
