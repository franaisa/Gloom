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

#include "Hikari.h"
#include "FlashValue.h"
#include "GUI/Server.h"
#include "Input/PlayerController.h"
#include "Input/Server.h"

namespace Application {

	
	bool CGameClientState::init(){
		
		_menuVisile = false;
		_netMgr = Net::CManager::getSingletonPtr();
		// Iniciamos el menu de seleccion de personaje
		_seleccion = GUI::CServer::getSingletonPtr()->addLayout("seleccion", Hikari::Position(Hikari::Center));
		_seleccion->load("SeleccionPersonaje.swf");
		_seleccion->bind("selected",Hikari::FlashDelegate(this, &CGameClientState::classSelected));
		_seleccion->hide();
		_seleccion->setTransparent(true, true);
		return true;
	}

	//______________________________________________________________________________

	void CGameClientState::activate() {
		CGameState::activate();
		
		// Mostramos el menú de selección de personaje
		
		_menuVisile = true;
		// Registramos a este estado como observador de red para que sea notificado
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
		// Indicamos que ya no queremos ser notificados por la red
		_netMgr->removeObserver(this);
		// Nos desconectamos
		_netMgr->deactivateNetwork();
		// Indicamos que ya no queremos ser notificados de la pulsación de teclas
		Input::CInputManager::getSingletonPtr()->removeKeyListener(this);

		CGameState::deactivate();

		Logic::CScoreboard::getSingletonPtr()->unLoadScoreboard();
		//GUI::CServer::getSingletonPtr()->destroyLayout(_seleccion);
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
			case Net::LOAD_WORLD_STATE: {

				Logic::CWorldState::getSingletonPtr()->deserialize(buffer);

				break;
			}

			case Net::LOAD_PLAYERS: {

				// Cargamos la informacion del player que nos han enviado
				Logic::TEntityID entityID;
				std::string playerClass, name;
				int nbPlayers;

				buffer.read(&nbPlayers, sizeof(nbPlayers));
				buffer.read(&entityID, sizeof(entityID));
				buffer.deserialize(name);
				buffer.deserialize(playerClass);

				// Llamo al metodo de creacion del jugador
				Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, playerClass, entityID);

				//Lo cargamos en el gestor de players, pero aqui tenemos que ver si el player ya existía antes, 
				//para en vez de re-cargarlo simplemente cambiarle la clase
				std::string playerName = player->getName();

				if(!Logic::CScoreboard::getSingletonPtr()->getPlayer(playerName))
					Logic::CScoreboard::getSingletonPtr()->addPlayer(playerName, player, playerClass);
				else
					Logic::CScoreboard::getSingletonPtr()->changePlayerEntity(playerName, player, playerClass);

				// No es necesario enviar confirmacion
				player->activate();
				player->start();

				break;
			}

			case Net::LOAD_LOCAL_PLAYER: {
				// Deserializamos la información de nuestro player
				Logic::TEntityID entityID;
				buffer.read(&entityID, sizeof(entityID));
				std::string playerClass, name;
				buffer.deserialize(name);
				buffer.deserialize(playerClass);

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

				//activamos la entidad
				player->activate();
				player->start();

				// Fijamos el objetivo de la camara
				Logic::CEntity* camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
				assert(camera != NULL && "Error: Esto no se puede hacer asi que sois unos lamers, ahora el servidor que hace?");
				camera->getComponent<Logic::CCamera>("CCamera")->setTarget(player);

				break;
			}
			case Net::LOAD_LOCAL_SPECTATOR: {
				// Deserializamos la info del espectador y lo cargamos
				Logic::TEntityID entityId;
				std::string nickname;
				
				buffer.read( &entityId, sizeof(entityId) );
				buffer.deserialize(nickname);
				
				Logic::CEntity* spectator = Logic::CServer::getSingletonPtr()->getMap()->createLocalPlayer(nickname, "Spectator", entityId);
				spectator->activate();
				spectator->start();

				Logic::CEntity* camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
				assert(camera != NULL && "Error, eres un lamer");
				camera->getComponent<Logic::CCamera>("CCamera")->setTarget(spectator);
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
			case Net::PLAYER_DISCONNECTED: {
				std::string name;
				buffer.deserialize(name);

				Logic::CScoreboard::getSingletonPtr()->deletePlayer(name);
			}
		}
	}

	//______________________________________________________________________________

	void CGameClientState::disconnectionPacketReceived(Net::CPaquete* packet) {
		std::cout << "Conexion con el servidor perdida" << std::endl;
		//disconnect();
		_app->setState("menu");
	}

	//______________________________________________________________________________

	bool CGameClientState::keyPressed(Input::TKey key) {
		CGameState::keyPressed(key);
		
		switch(key.keyId) {
			case Input::Key::C: {//cambio de clase
				//primero, quitamos al player de escuchar las teclas, para ello lo desactivamos del playerController
				Input::CServer::getSingletonPtr()->getPlayerController()->deactivate();
				//mostramos la gui
				_seleccion->show();
				_menuVisile = true;
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
		CGameState::keyReleased(key);

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
		
		if(!_menuVisile)
			return FLASH_VOID;
		
		int selectedClass = args.at(0).getNumber();
		Net::NetMessageType msgType = Net::CLASS_SELECTED;
		Net::CBuffer msg ( sizeof(msgType) + sizeof(selectedClass) );



 		switch(selectedClass) {
			case 0:
				if(Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar()) {
					Input::CServer::getSingletonPtr()->getPlayerController()->activate();
					_seleccion->hide();
					_menuVisile = false;
				} else {

				}

				break;
			case 1:
			case 2:
			case 3:
			case 4:

				_seleccion->hide();
				_menuVisile = false;
				//enviamos la clase elegida
				msg.serialize(msgType);
				msg.serialize(selectedClass);
				_netMgr->broadcast( msg.getbuffer(), msg.getSize() );

				if(Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar()){
					Input::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(NULL);
					Input::CServer::getSingletonPtr()->getPlayerController()->activate();
				}

				break;
			case 5: {
				_seleccion->hide();
				_menuVisile = false;
				//random de la clase y lo enviamos por red
				/*int randomclass = ((rand()*clock())%4)+1;
				msg.serialize(msgType);
				msg.serialize(randomclass);
				_netMgr->broadcast( msg.getbuffer(), msg.getSize() );
				*/

				// @deprecated Provisionalmente usamos el boton de random como boton
				// espectador
				Net::NetMessageType spectatorSelectedMsg = Net::SPECTATE_REQUEST;
				Net::CBuffer buffer ( sizeof(spectatorSelectedMsg) + sizeof(selectedClass) );
				buffer.write(&spectatorSelectedMsg, sizeof(spectatorSelectedMsg));
				buffer.serialize(selectedClass);
				_netMgr->broadcast( buffer.getbuffer(), buffer.getSize() );

				if(Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar()){
					Input::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(NULL);
					Input::CServer::getSingletonPtr()->getPlayerController()->activate();
				}

				break;
			}
		}//switch

		//enviamos el mensaje
		
		return FLASH_VOID;
	} // backReleased

};