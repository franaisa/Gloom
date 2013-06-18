//---------------------------------------------------------------------------
// LobbyClientState.cpp
//---------------------------------------------------------------------------

/**
@file LobbyClientState.cpp

Contiene la implementación del estado de lobby del cliente.

@see Application::CApplicationState
@see Application::CLobbyClientState

@author David Llansó
@date Agosto, 2010
*/

#include "LobbyClientState.h"
#include "GameClientState.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/Scoreboard.h"
#include "Logic/Maps/WorldState.h"

#include "Hikari.h"
#include "GUI/Server.h"

#include "Input/Server.h"

#include "Net/Manager.h"
#include "Net/paquete.h"
#include "Net/conexion.h"
#include "Net/buffer.h"

#include <iostream>
#include <windows.h>

using namespace std;

namespace Application {

	CLobbyClientState::CLobbyClientState(CBaseApplication *app) :	CApplicationState(app),
																	_netMgr(NULL),
																	loadThread(0){
		// Nada que hacer
	}

	//__________________________________________________________________

	CLobbyClientState::~CLobbyClientState() {
		// Nada que borrar
	}

	//__________________________________________________________________

	bool CLobbyClientState::init() {
		CApplicationState::init();

		_netMgr = Net::CManager::getSingletonPtr();

		// Cargamos la ventana que muestra el menú
		_menu = GUI::CServer::getSingletonPtr()->addLayoutToState(this,"joinGame", Hikari::Position(Hikari::Center));
		_menu->load("MultiplayerClient.swf");
		_menu->bind("connect",Hikari::FlashDelegate(this, &CLobbyClientState::startReleased));
		_menu->bind("back",Hikari::FlashDelegate(this, &CLobbyClientState::backReleased));
		_menu->hide();
		_loadMenu = GUI::CServer::getSingletonPtr()->addLayoutToState(this,"loadMenu", Hikari::Position(Hikari::Center));
		_loadMenu->load("LoadingMenu.swf");
		_loadMenu->hide();
		return true;
	} // init

	//__________________________________________________________________

	void CLobbyClientState::release() {
		CApplicationState::release();

		_netMgr = NULL;
	} // release

	//__________________________________________________________________

	void CLobbyClientState::activate() {
		CApplicationState::activate();

		// Mostramos el menu del cliente
		_menu->show();
		int menu = ((rand()*clock())%7)+1;
		_loadMenu->callFunction("loadMenu",Hikari::Args(menu));
		// Nos registramos como observadores de la red para ser notificados de
		// eventos y nos activamos como clientes.
		_netMgr->addObserver(this);
		_netMgr->activateAsClient();
	} // activate

	//__________________________________________________________________

	// Se llama al cambiar de estado
	void CLobbyClientState::deactivate() {
		// Ocultamos el menú de cliente
		_menu->hide();
		_loadMenu->hide();
		_netMgr->removeObserver(this);
		CApplicationState::deactivate();
		
	} // deactivate

	//__________________________________________________________________

	void CLobbyClientState::sendPlayerInfo() {
		// Obtenemos el nickname del player, que de momento es la única información asociada al
		// player que tenemos
		std::string playerNick = _menu->callFunction("getNick",Hikari::Args()).getString();

		// Enviamos los datos del player al servidor
		Net::NetMessageType msg = Net::PLAYER_INFO;
				
		// El tamaño del buffer es: cabecera + entero para el tam del nick + num de letras del nick
		Net::CBuffer playerData( sizeof(msg) + sizeof(unsigned int) + (playerNick.size() * sizeof(char)) );
		playerData.write( &msg, sizeof(msg) ); // Por problemas con enumerados serializamos manualmente
		playerData.serialize(playerNick, false);

		// Enviamos nuestra información al server
		_netMgr->broadcast( playerData.getbuffer(), playerData.getSize() );
	}

	//__________________________________________________________________

	bool CLobbyClientState::loadMap(const string& mapName) {
		return Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_client.txt")	&&
			   Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt")			&&
			   Logic::CServer::getSingletonPtr()->loadLevel(mapName + "_client.txt");
	}

	//__________________________________________________________________

	void CLobbyClientState::dataPacketReceived(Net::CPaquete* packet) {
		// Escribimos en un buffer los datos recibidos para ir leyendolos
		// poco a poco
		Net::CBuffer buffer( packet->getDataLength() );
		buffer.write(packet->getData(), packet->getDataLength());
		buffer.reset();

		// Obtenemos la cabecera del buffer que nos indica el tipo de mensaje
		// recibido
		Net::NetMessageType msg;
		buffer.read( &msg, sizeof(msg) );

		// En función del tipo de mensaje
		switch (msg) {
			case Net::SEND_PLAYER_INFO: {
				// Enviamos nuestros datos al servidor (nickname...)
				sendPlayerInfo();
				break;
			}
			case Net::LOAD_MAP: {
				// Obtenemos el nombre del mapa que está ejecutando el servidor
				string mapName;
				buffer.deserialize(_mapName);

				// La carga de entidades del mapa debe hacerse con las ids del servidor
				Logic::CEntityFactory::getSingletonPtr()->initDispatcher();

				// Si la carga del mapa tiene éxito, notificamos al servidor y continuamos. En 
				// caso contrario abortamos la ejecución.
				_loadMenu->show();
				
				_menu->hide();


				//creamos el thread que maneja la carga del mapa

				loadThread = std::make_shared<boost::thread>(&CLobbyClientState::loadMapThread , this);

				//if (WaitForSingleObject (loadHandle, INFINITE) == WAIT_OBJECT_0)
				//CloseHandle(loadHandle);

				/*if( loadMap(mapName) ) {
					// Avisamos de que hemos terminado la carga.
					Net::NetMessageType ackMsg = Net::MAP_LOADED;
					_netMgr->broadcast( &ackMsg, sizeof(ackMsg) );
				}
				else {
					_netMgr->removeObserver(this);
					_netMgr->deactivateNetwork();
					_app->exitRequest();
				}*/
				
				break;
			}
			case Net::GAME_SETTINGS: {
				buffer.read(&_gameMode, sizeof(_gameMode));
				buffer.read(&_gameTimeLeft, sizeof(_gameTimeLeft));
				buffer.read(&_goalScore, sizeof(_goalScore));

				Net::NetMessageType msgType = Net::GAME_SETTINGS_LOADED;

				_netMgr->broadcast(&msgType, sizeof(msgType));

				break;
			}
			case Net::LOAD_WORLD_STATE: {
				// Deserializar el estado del mundo
				Logic::CWorldState::getSingletonPtr()->deserialize(buffer);
				Logic::CScoreboard::getSingletonPtr()->deserialize(buffer);

				Net::NetMessageType ackMsg = Net::NetMessageType::WORLD_STATE_LOADED;
				_netMgr->broadcast( &ackMsg, sizeof(ackMsg) );
				break;
			}
			case Net::START_GAME: {
				// Dependiendo del modo saltamos a un estado u otro
				switch(_gameMode) {
					case GameMode::eDUEL:
						_app->setState("DUELClient");
						break;
					case GameMode::eDEATHMATCH:
						_app->setState("DMClient");
						break;
					case GameMode::eTEAM_DEATHMATCH:
						_app->setState("TDMClient");
						break;
					case GameMode::eCAPTURE_THE_FLAG:
						_app->setState("CTFClient");
						break;
				}

				Application::CGameClientState* nextState = static_cast<CGameClientState*>( _app->getNextState() );
				nextState->basicGameSettings(_gameTimeLeft, _goalScore);

				break;
			}
		}
	} // dataPacketReceived

	//__________________________________________________________________

	bool CLobbyClientState::keyPressed(Input::TKey key) {
	   return false;
	} // keyPressed

	//__________________________________________________________________

	bool CLobbyClientState::keyReleased(Input::TKey key) {
		switch(key.keyId) {
			case Input::Key::ESCAPE: {
				// Si pulsamos la tecla escape volvemos al menú de red
				// Indicamos que ya no queremos ser notificados de eventos de red
				// y desactivamos la red
				_netMgr->deactivateNetwork();
				_app->setState("netmenu");
				break;
			}
			case Input::Key::RETURN: {
				// Si pulsamos el enter intentamos conectarnos a la ip
				// dada
				requestConnectionTo(_menu->callFunction("getIp",Hikari::Args()).getString());
				break;
			}
			default: {
				return false;
			}
		}

		return true;
	} // keyReleased

	//__________________________________________________________________

	bool CLobbyClientState::mouseMoved(const Input::CMouseState &mouseState) {
		return true;
	} // mouseMoved

	//__________________________________________________________________

	bool CLobbyClientState::mousePressed(const Input::CMouseState &mouseState) {
		return true;
	} // mousePressed

	//__________________________________________________________________


	bool CLobbyClientState::mouseReleased(const Input::CMouseState &mouseState) {
		return true;
	} // mouseReleased

	//__________________________________________________________________

	Hikari::FlashValue CLobbyClientState::startReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args) {
		// Conectamos con la ip obtenida
		requestConnectionTo( args.at(0).getString() );

		return true;
	} // startReleased

	//__________________________________________________________________

	Hikari::FlashValue CLobbyClientState::backReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args) {
		// Volvemos al menú de red
		// Indicamos que ya no queremos ser notificados de eventos de red
		// y desactivamos la red
		_netMgr->deactivateNetwork();
		_app->setState("netmenu");
		
		return true;
	} // backReleased

	//__________________________________________________________________

	void CLobbyClientState::requestConnectionTo(const string& ip) {
		// Pedimos la conexion a la ip dada
		if ( !_netMgr->connectTo( (char*)ip.c_str(), 1234, 1) ) {
			_menu->callFunction( "enableButton", Hikari::Args() );
		}
	} // requestConnectionTo

	bool CLobbyClientState::loadMapThread(){
		return Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_client.txt")&&
			   Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt");
	}

	void CLobbyClientState::tick(unsigned int msecs){
		CApplicationState::tick(msecs);

		//asking the thread for completion ... if not the main thread crashes
		if (loadThread && loadThread->timed_join (boost::posix_time::milliseconds(0))){
			loadThread->join();
			loadThread = 0;

			// Avisamos de que hemos terminado la carga.
			Logic::CServer::getSingletonPtr()->loadLevel(_mapName + "_client.txt");
			Net::NetMessageType ackMsg = Net::MAP_LOADED;
			_netMgr->broadcast( &ackMsg, sizeof(ackMsg) );

			// Liberamos el dispatcher para las entidades por defecto
			Logic::CEntityFactory::getSingletonPtr()->releaseDispatcher();
			// Inicializamos el dispatcher de entidades lógicas en base a nuestro id de red y el
			// número de jugadores que hay en la partida
			// @deprecated lo ideal es que el server mande el numero de players de la partida
			Logic::CEntityFactory::getSingletonPtr()->initDispatcher( _netMgr->getID(), 12 );
		}
	}



} // namespace Application