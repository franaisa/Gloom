//---------------------------------------------------------------------------
// GameClientState.cpp
//---------------------------------------------------------------------------

/**
@file GameClientState.cpp

Contiene la implementación del estado de juego.

@see Application::CGameClientState
@see Application::CGameState

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "GameClientState.h"
#include "OgreClock.h"
#include "Net/Manager.h"
#include "Net/Servidor.h"
#include "Net/factoriared.h"
#include "Net/paquete.h"
#include "Net/buffer.h"

#include "MenuState.h"

#include "Input/InputManager.h"

#include "Logic/GameNetPlayersManager.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityID.h"
#include "Logic/Maps/Scoreboard.h"
#include "Logic/Entity/Components/Camera.h"
#include "Logic/Maps/WorldState.h"
#include "Logic/Entity/Components/CharacterName.h"

#include "Hikari.h"
#include "FlashValue.h"
#include "GUI/Server.h"
#include "Input/PlayerController.h"
#include "Input/Server.h"

namespace Application {

	
	bool CGameClientState::init() {
		_netMgr = Net::CManager::getSingletonPtr();
		// Iniciamos el menu de seleccion de personaje
		_seleccion = GUI::CServer::getSingletonPtr()->addLayout("seleccion", Hikari::Position(Hikari::Center), (unsigned short) 55);
		_seleccion->load("SeleccionPersonaje.swf");
		_seleccion->bind("selected",Hikari::FlashDelegate(this, &CGameClientState::classSelected));
		_seleccion->hide();
		_seleccion->setTransparent(true, true);

		_escmenu = GUI::CServer::getSingletonPtr()->addLayout("ScapeMenu", Hikari::Position(Hikari::Center), (unsigned short) 56);
		_escmenu->load("ESCMenu.swf");
		_escmenu->bind("exitClick",Hikari::FlashDelegate(this, &CGameClientState::exit));
		_escmenu->bind("resume",Hikari::FlashDelegate(this, &CGameClientState::resume));
		_escmenu->hide();
		_escmenu->setTransparent(true, true);

		return true;
	}

	//______________________________________________________________________________

	void CGameClientState::activate() {
		CGameState::activate();
		
		// Mostramos el menú de selección de personaje
		// Registramos a este estado como observador de red para que sea notificado
		// siempre y cuando acabemos de entrar en el modo online y no estuvieramos
		// previamente en un estado online
		_netMgr->addObserver(this);

		// Nos registramos como observadores del teclado
		Input::CInputManager::getSingletonPtr()->addKeyListener(this);
		
		// Configuramos los valores iniciales
		_reloj = clock();
		_npings = 0;
		_pingActual = 0;

		// Comenzamos el proceso de sincronizacion, para ello enviamos un mensaje de ping
		// y tomamos el tiempo para cronometrar cuanto tarda el servidor en respondernos
		Net::NetMessageType ackMsg = Net::PING;
		Net::NetID id = _netMgr->getID();
		Net::CBuffer ackBuffer(sizeof(ackMsg) + sizeof(id));
		ackBuffer.write(&ackMsg, sizeof(ackMsg));
		ackBuffer.write(&id, sizeof(id));
		
		_netMgr->broadcast(ackBuffer.getbuffer(), ackBuffer.getSize());

		Logic::CScoreboard::getSingletonPtr()->loadScoreboardDM();
		_seleccion->show();
		_seleccion->focus();
	} // activate

	//______________________________________________________________________________

	void CGameClientState::deactivate() {
		// Indicamos que ya no queremos ser notificados de la pulsación de teclas
		Input::CInputManager::getSingletonPtr()->removeKeyListener(this);

		Logic::CScoreboard::getSingletonPtr()->unLoadScoreboard();
		//GUI::CServer::getSingletonPtr()->destroyLayout(_seleccion);

		CGameState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	void CGameClientState::dataPacketReceived(Net::CPaquete* packet) {
		// Introducimos los datos recibidos en un buffer para leerlo
		// facilmente
		Net::CBuffer buffer( packet->getDataLength() );
		buffer.write( packet->getData(), packet->getDataLength() );
		buffer.reset();

		// Obtenemos la cabecera del mensaje
		Net::NetMessageType msg;
		buffer.read( &msg, sizeof(msg) );

		// Dependiendo del tipo de mensaje
		switch (msg) {
			case Net::LOAD_PLAYERS: {
				// Cargamos la informacion del player que nos han enviado
				Logic::TEntityID entityID;
				Logic::TeamFaction::Enum team;
				std::string playerClass, name;
				Net::NetID newPlayerNetId;
				int nbPlayers;

				buffer.read(&nbPlayers, sizeof(nbPlayers));
				buffer.read(&newPlayerNetId, sizeof(newPlayerNetId));
				buffer.read(&entityID, sizeof(entityID));
				buffer.deserialize(name);
				buffer.deserialize(playerClass);

				if(playerClass == "Spectator") {
					// Actualizamos el gestor de players
					Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
					if( !playersMgr->existsByNetId(newPlayerNetId) )
						playersMgr->addPlayer(newPlayerNetId, name);
					else
						playersMgr->setPlayerNickname(newPlayerNetId, name);
				
					playersMgr->setEntityID(newPlayerNetId, entityID);
					playersMgr->setPlayerTeam(newPlayerNetId, Logic::TeamFaction::eNONE);
					playersMgr->setPlayerState(newPlayerNetId, false);
					playersMgr->setFrags(newPlayerNetId, 0);
					playersMgr->setDeaths(newPlayerNetId, 0);

					// Pedimos al scoreboard que se borre este nombre por si acaso
					// hubiera habido algun jugador que haya pasado a modo espectador
					Logic::CScoreboard::getSingletonPtr()->deletePlayer(name);

					//@todo añadir al scoreboard como spectating
				}
				else {
					buffer.read(&team, sizeof(team));

					// Llamo al metodo de creacion del jugador
					Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, playerClass, entityID);

					//Lo cargamos en el gestor de players, pero aqui tenemos que ver si el player ya existía antes, 
					//para en vez de re-cargarlo simplemente cambiarle la clase
					std::string playerName = player->getName();

					if(!Logic::CScoreboard::getSingletonPtr()->getPlayer(playerName))
						Logic::CScoreboard::getSingletonPtr()->addPlayer(playerName, player, playerClass);
					else
						Logic::CScoreboard::getSingletonPtr()->changePlayerEntity(playerName, player, playerClass);

					// Actualizamos el gestor de players
					Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
					if( !playersMgr->existsByNetId(newPlayerNetId) )
						playersMgr->addPlayer(newPlayerNetId, name);
					else
						playersMgr->setPlayerNickname(newPlayerNetId, name);
				
					playersMgr->setEntityID(newPlayerNetId, entityID);
					playersMgr->setPlayerTeam(newPlayerNetId, team);
					playersMgr->setPlayerState(newPlayerNetId, true);
					playersMgr->setFrags(newPlayerNetId, 0);
					playersMgr->setDeaths(newPlayerNetId, 0);

					// No es necesario enviar confirmacion
					player->activate();
					player->start();
				}
				
				break;
			}

			case Net::LOAD_LOCAL_PLAYER: {

				// Deserializamos la información de nuestro player
				Net::NetID newPlayerNetId;
				buffer.read(&newPlayerNetId, sizeof(newPlayerNetId));
				Logic::TEntityID entityID;
				buffer.read(&entityID, sizeof(entityID));
				std::string playerClass, name;
				buffer.deserialize(name);
				buffer.deserialize(playerClass);

				Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
				_seleccion->hide();
				if(playerClass == "Spectator") {
					Logic::CEntity* spectator = Logic::CServer::getSingletonPtr()->getMap()->createLocalPlayer(name, playerClass, entityID);
					spectator->activate();
					spectator->start();

					Logic::CEntity* camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
					assert(camera != NULL && "Error, eres un lamer");
					camera->getComponent<Logic::CCamera>("CCamera")->setTarget(spectator);

					// Actualizamos el gestor de players
					if( !playersMgr->existsByNetId(newPlayerNetId) )
						playersMgr->addPlayer(newPlayerNetId, name);
					else
						playersMgr->setPlayerNickname(newPlayerNetId, name);
				
					playersMgr->setEntityID(newPlayerNetId, entityID);
					playersMgr->setPlayerTeam(newPlayerNetId, Logic::TeamFaction::eNONE);
					playersMgr->setPlayerState(newPlayerNetId, false);
					playersMgr->setFrags(newPlayerNetId, 0);
					playersMgr->setDeaths(newPlayerNetId, 0);

					// Pedimos al scoreboard que se borre este nombre por si acaso
					// hubiera habido algun jugador que haya pasado a modo espectador
					Logic::CScoreboard::getSingletonPtr()->deletePlayer(name);

					//@todo añadir al scoreboard como spectating

					// Recorremos cada una de las entidades del gestor de players, para
					// indicarles que nosotros somos el player para saber como deberiamos
					// visualizar sus nombres
					auto it = playersMgr->begin();
					for(; it != playersMgr->end(); ++it) {
						if( it->isSpawned() ) {
							Logic::CEntity* player = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID( it->getEntityId().first );
							player->getComponent<Logic::CCharacterName>("CCharacterName")->setVisible(true);
						}
					}
				}
				else {
					Logic::TeamFaction::Enum team;
					buffer.read(&team, sizeof(team));

					// Creamos al jugador como local (es decir, lo seteamos
					// como el jugador controlado por las teclas).
					Logic::CEntity* player = Logic::CServer::getSingletonPtr()->getMap()->createLocalPlayer(name, playerClass, entityID);

					//Lo cargamos en el gestor de players, pero aqui tenemos que ver si el player ya existía antes, 
					//para en vez de re-cargarlo simplemente cambiarle la clase
					std::string playerName = player->getName();

					if(!Logic::CScoreboard::getSingletonPtr()->getPlayer(playerName))
						Logic::CScoreboard::getSingletonPtr()->addLocalPlayer(playerName, player, playerClass);
					else
						Logic::CScoreboard::getSingletonPtr()->changePlayerEntity(playerName, player, playerClass);

					// Actualizamos el gestor de players
					if( !playersMgr->existsByNetId(newPlayerNetId) )
						playersMgr->addPlayer(newPlayerNetId, name);
					else
						playersMgr->setPlayerNickname(newPlayerNetId, name);
				
					playersMgr->setEntityID(newPlayerNetId, entityID);
					playersMgr->setPlayerTeam(newPlayerNetId, team);
					playersMgr->setPlayerState(newPlayerNetId, true);
					playersMgr->setFrags(newPlayerNetId, 0);
					playersMgr->setDeaths(newPlayerNetId, 0);

					//activamos la entidad
					player->activate();
					player->start();

					// Fijamos el objetivo de la camara
					Logic::CEntity* camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
					assert(camera != NULL && "Error: Esto no se puede hacer asi que sois unos lamers, ahora el servidor que hace?");
					camera->getComponent<Logic::CCamera>("CCamera")->setTarget(player);

					//ack message to avoid deads
					Net::NetMessageType msgping = Net::LOCAL_PLAYER_LOADED;
					Logic::TEntityID id = player->getEntityID();
					Net::CBuffer ackBuffer(sizeof(msgping) + sizeof(id));
					ackBuffer.write(&msgping, sizeof(msgping));
					ackBuffer.write(&id, sizeof(id));
					_netMgr->broadcast(ackBuffer.getbuffer(), ackBuffer.getSize());

					// Recorremos cada una de las entidades del gestor de players, para
					// indicarles que nosotros somos el player para saber como deberiamos
					// visualizar sus nombres
					Logic::TeamFaction::Enum myTeam = playersMgr->getTeamUsingEntityId(entityID);

					auto it = playersMgr->begin();
					for(; it != playersMgr->end(); ++it) {
						
						if( it->isSpawned() ) {
							Logic::TEntityID id = it->getEntityId().first;

							if(entityID != id) {
								Logic::CEntity* player = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);
								Logic::TeamFaction::Enum playerTeam = playersMgr->getTeamUsingEntityId(id);

								if(myTeam == Logic::TeamFaction::eNONE || playerTeam == Logic::TeamFaction::eNONE || myTeam == playerTeam)
									player->getComponent<Logic::CCharacterName>("CCharacterName")->setVisible(true);
							}
						}
					}
				}

				Input::CPlayerController* playerController = Input::CServer::getSingletonPtr()->getPlayerController();
				if( playerController->getControllerAvatar() )
					playerController->activate();

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
					Net::NetID id = _netMgr->getID();
					Net::CBuffer ackBuffer(sizeof(msgping) + sizeof(id));
					ackBuffer.write(&msgping, sizeof(msgping));
					ackBuffer.write(&id, sizeof(id));
					_reloj = clock();
					_netMgr->broadcast(ackBuffer.getbuffer(), ackBuffer.getSize());
				}

				break;
			}
			case Net::PLAYER_OFF_MATCH: {
				Net::NetID playerNetId;
				buffer.read(&playerNetId, sizeof(playerNetId));
				std::string name;
				buffer.deserialize(name);

				// Por si acaso estaba registrado
				Logic::CScoreboard::getSingletonPtr()->deletePlayer(name);

				// Actualizamos el gestor de players
				Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
				if( playersMgr->existsByNetId(playerNetId) )
					playersMgr->removePlayer(playerNetId);

				break;
			}
			case Net::NO_FREE_PLAYER_SLOTS: {
				std::cerr << "Error: There are no available player slots" << std::endl;
				_seleccion->callFunction("playerFull");
				break;
			}
			case Net::NO_FREE_SPECTATOR_SLOTS: {
				std::cerr << "Error: There are no available spectator slots" << std::endl;
				_seleccion->callFunction("spectatorFull");
				break;
			}
			case Net::PLAYER_KICK: {
				std::cerr << "You have been kicked" << std::endl;
				disconnect();
				_app->setState("menu");
				break;
			}
		}
	}

	//______________________________________________________________________________

	void CGameClientState::disconnect() {
		// Indicamos que ya no queremos ser notificados por la red
		_netMgr->removeObserver(this);
		// Nos desconectamos
		_netMgr->deactivateNetwork();
	}

	//______________________________________________________________________________

	void CGameClientState::disconnectionPacketReceived(Net::CPaquete* packet) {
		std::cout << "NETWORK: Server is offline" << std::endl;
		disconnect();
		_app->setState("menu");
		Application::CMenuState* state = static_cast<CMenuState*>( _app->getState( "menu" ) );
		state->netError();
	}

	//______________________________________________________________________________

	bool CGameClientState::keyPressed(Input::TKey key) {
		switch(key.keyId) {
			case Input::Key::ESCAPE: {
				if( _seleccion->getVisibility() ) {
					_seleccion->hide();
				}else if ( _escmenu->getVisibility() ) {
					
					//_escmenu->hide();

				}else {
					_escmenu->show();
					Input::CServer::getSingletonPtr()->getPlayerController()->deactivate();
				}

				break;
			}
			case Input::Key::C: {//cambio de clase
				//primero, quitamos al player de escuchar las teclas, para ello lo desactivamos del playerController
				Input::CServer::getSingletonPtr()->getPlayerController()->deactivate();
				//mostramos la gui
				_seleccion->show();
				break;
			}
			default: {
				return true;
			}
		}
		
		return true;
	} // keyPressed

	//______________________________________________________________________________

	bool CGameClientState::keyReleased(Input::TKey key) {
		/*switch(key.keyId) {
			case Input::Key::ESCAPE: {
				if( _seleccion->getVisibility() ) {
					_seleccion->hide();
				}else if ( _escmenu->getVisibility() ) {
					
					_escmenu->hide();

				}else {
					_escmenu->show();
				}

				break;
			}
			default: {
				return false;
			}
		}*/

		return true;
	} // keyReleased

	//______________________________________________________________________________
	
	bool CGameClientState::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//______________________________________________________________________________
		
	bool CGameClientState::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//______________________________________________________________________________

	bool CGameClientState::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased

	//______________________________________________________________________________

	Hikari::FlashValue CGameClientState::classSelected(Hikari::FlashControl* caller, const Hikari::Arguments& args) {
		
		std::string sClass = args.at(0).getString();
		Net::NetMessageType msgType = Net::CLASS_SELECTED;
		

		int selectedClass;

		if(sClass == "screamer"){
			selectedClass = 1;
		} else if(sClass == "hound"){
			selectedClass = 2;
		} else if(sClass == "archangel"){
			selectedClass = 3;
		} else if(sClass == "shadow"){
			selectedClass = 4;
		} else if(sClass == "spectator"){
			selectedClass = 5;
		}else if(sClass == "cancel"){
			selectedClass = 0;
		}

		Net::CBuffer msg(sizeof(msgType) + sizeof(selectedClass));
		Input::CPlayerController* playerController = Input::CServer::getSingletonPtr()->getPlayerController();
 		switch(selectedClass) {
			case 0: {

				if( playerController->getControllerAvatar() ) {
					playerController->activate();
					_seleccion->hide();
				} 

				break;
			}
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:

				//enviamos la clase elegida
				msg.serialize(msgType);
				msg.serialize(selectedClass);
				_netMgr->broadcast( msg.getbuffer(), msg.getSize() );

				break;
		}//switch

		//enviamos el mensaje
		
		return FLASH_VOID;
	} // backReleased

	Hikari::FlashValue CGameClientState::exit(Hikari::FlashControl* caller, const Hikari::Arguments& args){
		disconnect();
		_app->setState("menu");
		_escmenu->hide();

		return FLASH_VOID;
	}

	Hikari::FlashValue CGameClientState::resume(Hikari::FlashControl* caller, const Hikari::Arguments& args){
		Input::CServer::getSingletonPtr()->getPlayerController()->activate();
		_escmenu->hide();

		return FLASH_VOID;
	}
	//______________________________________________________________________________

	void CGameClientState::basicGameSettings(unsigned int gameTime, unsigned int goalScore) {
		_gameTime = gameTime;
		_goalScore = goalScore;

		_infiniteScore = _goalScore == 0;
		_infiniteTime = _gameTime == 0;
	}

};