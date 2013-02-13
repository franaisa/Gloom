//---------------------------------------------------------------------------
// LobbyServerState.cpp
//---------------------------------------------------------------------------

/**
@file LobbyServerState.cpp

Contiene la implementación del estado de lobby del servidor.

@see Application::CApplicationState
@see Application::CLobbyServerState

@author David Llansó
@date Agosto, 2010
*/

#include "LobbyServerState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityID.h"

#include "Logic/Entity/Entity.h"
#include "GUI/Server.h"
#include "Net/Manager.h"
#include "Net/Servidor.h"
#include "Net/factoriared.h"
#include "Net/paquete.h"
#include "Net/buffer.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

#include "Logic/GameNetPlayersManager.h"

namespace Application {

	CLobbyServerState::~CLobbyServerState() 
	{
	} // ~CLobbyServerState

	//--------------------------------------------------------

	bool CLobbyServerState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("NetLobbyServer.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyServerState::startReleased, this));

		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(false);
		
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyServerState::backReleased, this));
	
		return true;

	} // init

	//--------------------------------------------------------

	void CLobbyServerState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CLobbyServerState::activate() 
	{
		CApplicationState::activate();
		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

		Net::CManager::getSingletonPtr()->addObserver(this);
		Net::CManager::getSingletonPtr()->activateAsServer(1234,16);

		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")
			->setText("Status: Server up. Waiting for clients ...");
	} // activate

	//--------------------------------------------------------

	void CLobbyServerState::deactivate() 
	{		
		Net::CManager::getSingletonPtr()->removeObserver(this);
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CLobbyServerState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CLobbyServerState::keyPressed(GUI::TKey key)
	{
	   return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CLobbyServerState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->setState("netmenu");
			break;
		case GUI::Key::RETURN:
			doStart();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CLobbyServerState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLobbyServerState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CLobbyServerState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CLobbyServerState::startReleased(const CEGUI::EventArgs& e)
	{
		doStart();
		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CLobbyServerState::backReleased(const CEGUI::EventArgs& e)
	{
		Net::CManager::getSingletonPtr()->deactivateNetwork();
		_app->setState("netmenu");
		return true;

	} // backReleased

	//--------------------------------------------------------

	void CLobbyServerState::doStart()
	{
		_waiting = false;

		// Construimos un buffer para alojar el mensaje de carga de mapa que
		// enviaremos a todos los clientes mediante send
		Net::NetMessageType msg = Net::LOAD_PLAYER_INFO;
		Net::CBuffer buffer(sizeof(msg));
		buffer.write(&msg, sizeof(msg));

		Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize());

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_server.txt")) {
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();
		}

		/*if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt")) {
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();
		}*/

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel("map_server.txt")) {
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();
		}
	} // doStart

	//--------------------------------------------------------

	void CLobbyServerState::dataPacketReceived(Net::CPaquete* packet)
	{
		Net::NetID playerId = packet->getConexion()->getId();

		Net::CBuffer buffer(packet->getDataLength());
		buffer.write(packet->getData(), packet->getDataLength());
		// Desplazamos el puntero al principio para realizar la lectura
		buffer.reset();

		Net::NetMessageType msg;
		//memcpy(&msg, packet->getData(), sizeof(msg));
		buffer.read(&msg, sizeof(msg));
		
		switch (msg)
		{
		case Net::PLAYER_INFO:
		{
			std::string playerNick, playerMesh;
			buffer.deserialize(playerNick);
			buffer.deserialize(playerMesh);

			std::cout << "Se quiere conectar " << playerNick << std::endl;
			std::cout << "Con la maya " << playerMesh << std::endl;

			Logic::CGameNetPlayersManager::getSingletonPtr()->setPlayerNickname(playerId, playerNick);
			Logic::CGameNetPlayersManager::getSingletonPtr()->setPlayerMesh(playerId, playerMesh);

			// Si se ha cargado la información de todos los clientes, entonces
			// comenzamos la fase de carga del mapa

			if( ++_playersFetched == Logic::CGameNetPlayersManager::getSingletonPtr()->getNumberOfPlayersConnected() ) {
				Net::NetMessageType msg = Net::LOAD_MAP;
				Net::CBuffer buffer(sizeof(msg));
				buffer.write(&msg, sizeof(msg));

				Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize());
			}

			break;
		}
		case Net::MAP_LOADED:
		{
			//Almacenamos el ID del usuario que se ha cargado el mapa.
			_mapLoadedByClients.push_back(packet->getConexion()->getId());

			//Si todos los clientes han cargado los mapas pasamos a crear jugadores.
			if(_clients.size() == _mapLoadedByClients.size()) {
				// Se debe crear un jugador por cada cliente registrado.
				for(TNetIDList::const_iterator it = _clients.begin(); it != _clients.end(); it++) {
					//Preparamos la lista de control de carga de jugadores.
					//Esto quiere decir que el cliente (*it) ha cargado 0 jugadores
					TNetIDCounterPair elem(*it,0);
					_playersLoadedByClients.insert(elem);

					// @todo Hay que enviar un paquete tipo LOAD_PLAYER con 
					// el NetID del cliente del que estamos creando el jugador (*it)
					Net::NetMessageType msg = Net::LOAD_PLAYER;
					Net::NetID netId = *it;
					
					Net::CBuffer buffer(sizeof(msg) + sizeof(netId) + sizeof(Logic::EntityID));
					buffer.write(&msg, sizeof(msg));
					buffer.write(&netId, sizeof(netId));

					// Mandar el ID del cliente actual a todos los clientes para que
					// lo carguen
					
					// Creamos el player. Deberíamos poder propocionar caracteríasticas
					// diferentes según el cliente (nombre, modelo, etc.). Esto es una
					// aproximación, solo cambiamos el nombre y decimos si es el jugador
					// local
					std::string name("Player");
					std::stringstream number;
					number << (*it);
					name.append(number.str());

					// @todo Llamar al método de creación del jugador. Deberemos decidir
					// si el jugador es el jugador local. Al ser el servidor ninguno lo es

					Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name);

					Logic::TEntityID id = player->getEntityID();
					buffer.write(&id, sizeof(id));

					Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize());
				}
			}
			break;
		}
		case Net::PLAYER_LOADED:
		{
			//Aumentamos el número de jugadores cargados por el cliente
			(*_playersLoadedByClients.find(packet->getConexion()->getId())).second++;

			// @todo Comprobar si todos los clientes han terminado de 
			// cargar todos los jugadores
			bool loadFinished = true;
			TNetIDCounterMap::const_iterator it2;
			for(TNetIDList::const_iterator it = _clients.begin(); it != _clients.end() && loadFinished; it++) {
				// Obtener la pareja (NetID, jugadores cargados)
				it2 = _playersLoadedByClients.find(*it);
				
				if(it2 != _playersLoadedByClients.end()) {
					// Si el numero de jugadores cargados para este cliente es menor
					// que el numero total de clientes cargar, entonces es que aun
					// hay clientes que no han cargado a todos los jugadores
					if(it2->second < _clients.size()) {
						loadFinished = false;
					}
				}
			}
			//Si todos los clientes han cargado todos los players
			if(loadFinished)
			{
				//Enviamos el mensaje de que empieza el juego a todos los clientes
				Net::NetMessageType msg = Net::START_GAME;
				Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg));
				_app->setState("multiplayerTeamDeathmatchServer");
			}
			break;
		}
		}

	} // dataPacketReceived

	//--------------------------------------------------------

	void CLobbyServerState::connexionPacketReceived(Net::CPaquete* packet)
	{
		if(_waiting)
		{
			//Mostramos un poco de información en el status
			char id[100];
			unsigned int ip = packet->getConexion()->getAddress();
			byte* p = (byte*)&ip;
			sprintf_s(id,"\nClient conected: %d.%d.%d.%d:%d\nGame can start...\n",p[0],p[1],p[2],p[3], packet->getConexion()->getPort()); 				
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->appendText(id);
			
			//Habilitamos el botón de start.
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(true);

			//Almacenamos el ID del usuario que se ha conectado.
			_clients.push_back(packet->getConexion()->getId());

			// Añadimos un nuevo jugador para esta nueva conexion
			// En la fase de carga de datos incluiremos su nombre y modelo
			Logic::CGameNetPlayersManager::getSingletonPtr()->addPlayer( packet->getConexion()->getId() );
		}

	} // connexionPacketReceived

	//--------------------------------------------------------

	void CLobbyServerState::disconnexionPacketReceived(Net::CPaquete* packet)
	{
		// @todo gestionar desconexiones.
		// TODO gestionar desconexiones.
		//Eliminamos el ID del usuario que se ha desconectado.
		_clients.remove( packet->getConexion()->getId() );
		_mapLoadedByClients.remove( packet->getConexion()->getId() );

		TNetIDCounterMap::const_iterator pairIt = _playersLoadedByClients.find(packet->getConexion()->getId());
		if(pairIt != _playersLoadedByClients.end())
			_playersLoadedByClients.erase(pairIt);

		if(!_clients.empty()) {
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")
				->setText("Client disconnected. Waiting for new clients...");					
		} else{
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")
				->setText("All clients disconnected. Waiting for some client...");
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(false); //Dehabilitamos el botón de start.
		}

	} // disconnexionPacketReceived

} // namespace Application
