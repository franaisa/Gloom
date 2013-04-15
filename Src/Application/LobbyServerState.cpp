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
#include "Hikari.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <direct.h>

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
		_menu = GUI::CServer::getSingletonPtr()->addLayoutToState(this,"server", Hikari::Position(Hikari::Center));
		_menu->load("MultiplayerServer.swf");
		_menu->bind("back",Hikari::FlashDelegate(this, &CLobbyServerState::backReleased));
		_menu->bind("newgame",Hikari::FlashDelegate(this, &CLobbyServerState::startReleased));
		_menu->bind("createGame",Hikari::FlashDelegate(this, &CLobbyServerState::createReleased));
		_menu->hide();
		listFiles();
	
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
		_menu->show();

		
	} // activate

	//--------------------------------------------------------

	void CLobbyServerState::deactivate() 
	{	
		CApplicationState::deactivate();

		Net::CManager::getSingletonPtr()->removeObserver(this);
		_menu->hide();

	} // deactivate

	//--------------------------------------------------------

	void CLobbyServerState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CLobbyServerState::keyPressed(Input::TKey key)
	{
	   return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CLobbyServerState::keyReleased(Input::TKey key)
	{
		switch(key.keyId)
		{
		case Input::Key::ESCAPE:
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->setState("netmenu");
			break;
		case Input::Key::RETURN:
			doStart();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CLobbyServerState::mouseMoved(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLobbyServerState::mousePressed(const Input::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CLobbyServerState::mouseReleased(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	Hikari::FlashValue CLobbyServerState::startReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		doStart();
		return FLASH_VOID;

	} // startReleased
			
	//--------------------------------------------------------

	Hikari::FlashValue CLobbyServerState::backReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		Net::CManager::getSingletonPtr()->deactivateNetwork();
		_app->setState("netmenu");
		return FLASH_VOID;

	} // backReleased

	Hikari::FlashValue CLobbyServerState::createReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		
		//ponemos al server a escuchar
		Net::CManager::getSingletonPtr()->addObserver(this);
		Net::CManager::getSingletonPtr()->activateAsServer(1234,16);

		//deshabilitamos el boton crear partida
		_menu->callFunction("disableCreate",Hikari::Args());
		

		//cogemos el mapa en el que crear la partida
		_map=args.at(0).getString();

		//mostramos en el gui lo que esta pasando
		_menu->callFunction("pushCommand",Hikari::Args("Status: Server up. Waiting for clients ..."));
		return FLASH_VOID;

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

		if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt")) {
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();
		}

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel(_map+"_server.txt")) {
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
			// Deserializamos el nombre del player y el mesh (en un futuro la clase del player)
			std::string playerNick, playerMesh;
			buffer.deserialize(playerNick);
			buffer.deserialize(playerMesh);

			// Registramos estos datos en el gestor de players
			Logic::CGameNetPlayersManager::getSingletonPtr()->setPlayerNickname(playerId, playerNick);
			Logic::CGameNetPlayersManager::getSingletonPtr()->setPlayerMesh(playerId, playerMesh);

			// Si se ha cargado la información de todos los clientes, entonces
			// comenzamos la fase de carga del mapa
			if( ++_playersFetched == Logic::CGameNetPlayersManager::getSingletonPtr()->getNumberOfPlayersConnected() ) {
				Net::NetMessageType msg = Net::LOAD_MAP;
				Net::CBuffer buffer(sizeof(msg)+_map.size()*sizeof(char));
				buffer.write(&msg, sizeof(msg));
				buffer.serialize(_map,false);

				Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize());
			}

			break;
		}
		case Net::MAP_LOADED:
		{
			//Enviamos el mensaje de que empieza el juego a todos los clientes
			Net::NetMessageType msg = Net::START_GAME;
			Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg));
			_app->setState("multiplayerTeamDeathmatchServer");
			break;
		}
		}//switch

	} // dataPacketReceived

	//--------------------------------------------------------

	void CLobbyServerState::connexionPacketReceived(Net::CPaquete* packet)
	{
		if(_waiting)
		{
			//Mostramos un poco de información en el status
			char id[100];
			unsigned int ip = packet->getConexion()->getAddress();
			
			BYTE* p = (BYTE*)&ip;
			sprintf_s(id,"\nClient conected: %d.%d.%d.%d:%d\nGame can start...\n",p[0],p[1],p[2],p[3], packet->getConexion()->getPort()); 				
			_menu->callFunction("pushCommand",Hikari::Args(id));
			
			//Habilitamos el botón de start.
			_menu->callFunction("enableBegin",Hikari::Args());
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

		// Eliminamos la informacion del player que se quiere desconectar del gestor de players
		// y ademas eliminamos este player de la lista de players cargados del resto de clientes
		// si es que han llegado a cargarlo
		Logic::CGameNetPlayersManager::getSingletonPtr()->removePlayer( packet->getConexion()->getId() );
		--_playersFetched;

		// @todo
		// NOTIFICAR A LOS CLIENTES PARA QUE ELIMINEN ESE JUGADOR DE LA PARTIDA

		TNetIDCounterMap::const_iterator pairIt = _playersLoadedByClients.find(packet->getConexion()->getId());
		if(pairIt != _playersLoadedByClients.end())
			_playersLoadedByClients.erase(pairIt);

		if(!_clients.empty()) {
			_menu->callFunction("pushCommand",Hikari::Args("Client disconnected. Waiting for new clients..."));
		} else{
			_menu->callFunction("pushCommand",Hikari::Args("All clients disconnected. Waiting for some client..."));

			//Dehabilitamos el botón de start.
			_menu->callFunction("disableBegin",Hikari::Args()); 
		}

	} // disconnexionPacketReceived

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLobbyServerState::listFiles(){
		WIN32_FIND_DATA FindData;
		HANDLE hFind;
		hFind = FindFirstFile("media\\maps\\*.txt", &FindData);

		std::string filename;
		while (FindNextFile(hFind, &FindData))
		{     
			filename = FindData.cFileName;
			_menu->callFunction("pushFile",Hikari::Args(filename.substr(0,filename.find(".txt"))));

		}
	}

} // namespace Application
