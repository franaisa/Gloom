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

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "Hikari.h"
#include "GUI/Server.h"

#include "Input/Server.h"

#include "Net/Manager.h"
#include "Net/paquete.h"
#include "Net/conexion.h"
#include "Net/buffer.h"

#include <iostream>

using namespace std;

namespace Application {

	CLobbyClientState::CLobbyClientState(CBaseApplication *app) : CApplicationState(app),
																  _netMgr(NULL) {
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
				buffer.deserialize(mapName);

				// Inicializamos el dispatcher de entidades lógicas en base a nuestro id de red y el
				// número de jugadores que hay en la partida
				// @deprecated lo ideal es que el server mande el numero de players de la partida
				Logic::CEntityFactory::getSingletonPtr()->initDispatcher( _netMgr->getID(), 12 );

				// Si la carga del mapa tiene éxito, notificamos al servidor y continuamos. En 
				// caso contrario abortamos la ejecución.
				if( loadMap(mapName) ) {
					// Avisamos de que hemos terminado la carga.
					Net::NetMessageType ackMsg = Net::MAP_LOADED;
					_netMgr->broadcast( &ackMsg, sizeof(ackMsg) );
				}
				else {
					_netMgr->removeObserver(this);
					_netMgr->deactivateNetwork();
					_app->exitRequest();
				}

				break;
			}
			case Net::LOAD_PLAYERS: {
				// Cargamos los players que ya estaban en la partida
				int nbPlayers;
				Logic::TEntityID entityID;
				Logic::CEntity* player;
				string playerClass, name;

				// Obtenemos el número de jugadores conectados
				buffer.read(&nbPlayers, sizeof(nbPlayers));
				for(int i = 0; i < nbPlayers; ++i) {
					buffer.read(&entityID, sizeof(entityID));
					buffer.deserialize(name);
					buffer.deserialize(playerClass);

					// Llamo al metodo de creacion del jugador
					player = Logic::CServer::getSingletonPtr()->getMap()->
							 createPlayer(name, playerClass, entityID);
				}
				
				// Confirmamos de que se han cargado todos los players con exito
				Net::NetMessageType ackMsg = Net::PLAYERS_LOADED;
				_netMgr->broadcast( &ackMsg, sizeof(ackMsg) );
				
				break;
			}
			case Net::LOAD_WORLD_STATE: {
				// Deserializar el estado del mundo
				Net::NetMessageType ackMsg = Net::NetMessageType::WORLD_STATE_LOADED;
				_netMgr->broadcast( &ackMsg, sizeof(ackMsg) );
				break;
			}
			case Net::START_GAME: {
				_app->setState("gameClient");
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
				_netMgr->removeObserver(this);
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
		_netMgr->removeObserver(this);
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

} // namespace Application