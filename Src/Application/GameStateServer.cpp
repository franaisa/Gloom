//---------------------------------------------------------------------------
// GameState.cpp
//---------------------------------------------------------------------------

/**
@file GameState.cpp

Contiene la implementación del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llansó
@date Agosto, 2010
*/

#include "GameStateServer.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "Physics/Server.h"

#include "Net/Manager.h"
#include "Net/Cliente.h"
#include "Net/factoriared.h"
#include "Net/paquete.h"
#include "Net/conexion.h"
#include "Net/buffer.h"

//#include <CEGUISystem.h>
//#include <CEGUIWindowManager.h>
//#include <CEGUIWindow.h>

namespace Application {

	bool CGameStateServer::init() 
	{
		// Ejecutamos la inicialización de la clase padre
		// En este caso no hace nada, solo retorna true
		CApplicationState::init();

		// INICIALIZACIÓN DE LA FÍSICA
		// ---------------------------

		// TODO: desactivar colisiones entre los grupos 0 y 1
		//Physics::CServer::getSingletonPtr()->setGroupCollisions(0, 1, false);

		// TODO: Crear la escena física usando el servidor de física
		//Physics::CServer::getSingletonPtr()->createScene();

		// INICIALIZACIÓN DE LA LÓGICA
		// ---------------------------

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		/*if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel("map.txt"))
			return false;*/

		// INICIALIZACIÓN DEL GUI
		// ----------------------

		// Ahora mismo la implementación está totalmente acoplada a CEGUI
		// Hay que desacoplarlo utilizando un nuevo paquete donde se abstraiga
		// el subsistema utilizado

		// Cargamos la ventana que muestra el tiempo de juego transcurrido.
		//CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Time.layout");
		//_timeWindow = CEGUI::WindowManager::getSingleton().getWindow("Time");
		//LAYOUT TIME
		//GUI::CServer::getSingletonPtr()->addLayoutToState(this, "Time");
		
		//LAYOUT MIRA
		//GUI::CServer::getSingletonPtr()->addLayoutToState(this, "Mira");

		return true;
	} // init

	//--------------------------------------------------------

	void CGameStateServer::release() 
	{
		// Liberar los recursos reservados para la escena y las
		// clases construidas a partir de los blueprints
		Logic::CServer::getSingletonPtr()->unLoadLevel();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		// Liberar la escena física usando el motor de física
		Physics::CServer::getSingletonPtr()->destroyScene();

		// Llamar al método padre por si acaso tiene que hacer algo
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CGameStateServer::activate() 
	{
		CApplicationState::activate();
		
		// Activamos el mapa que ha sido cargado para la partida.
		Logic::CServer::getSingletonPtr()->activateMap();

		// Queremos que el GUI maneje al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->activate();

		Net::CManager::getSingletonPtr()->addObserver(this);

		// Activamos la ventana que nos muestra el tiempo transcurrido.
		//CEGUI::System::getSingletonPtr()->setGUISheet(_timeWindow);
		//_timeWindow->setVisible(true);
		//_timeWindow->activate();

		//LAYOUT TIME
		//GUI::CServer::getSingletonPtr()->activateGUI(this, "Time");
	
		//LAYOUT MIRA
		//GUI::CServer::getSingletonPtr()->activateGUI(this, "Mira");
	} // activate

	//--------------------------------------------------------

	void CGameStateServer::deactivate() 
	{
		Net::CManager::getSingletonPtr()->removeObserver(this);
		Net::CManager::getSingletonPtr()->deactivateNetwork();

		// Desactivamos la ventana de tiempo.
		//_timeWindow->deactivate();
		//_timeWindow->setVisible(false);

		//LAYOUT TIME
		//GUI::CServer::getSingletonPtr()->deactivateGUI();

		// Desactivamos la clase que procesa eventos de entrada para 
		// controlar al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();
		
		// Desactivamos el mapa de la partida.
		Logic::CServer::getSingletonPtr()->deactivateMap();
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameStateServer::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		// TODO: realizar la simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;
		
		std::stringstream text;
		text << "Time: " << _time/1000;

		//TEXTO DEL LAYOUT TIME
		//GUI::CServer::getSingletonPtr()->setText(text.str());

	} // tick

	//--------------------------------------------------------

	bool CGameStateServer::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameStateServer::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->setState("menu");
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameStateServer::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameStateServer::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameStateServer::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

	//--------------------------------------------------------

	/*void CGameStateServer::doStart()
	{
		_waiting = false;

		// Construimos un buffer para alojar el mensaje de carga de mapa que
		// enviaremos a todos los clientes mediante send
		Net::NetMessageType msg = Net::LOAD_MAP;
		Net::CBuffer buffer(sizeof(msg));
		buffer.write(&msg, sizeof(msg));

		Net::CManager::getSingletonPtr()->send(buffer.getbuffer(), buffer.getSize());

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_server.txt"))
		{
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();
		}

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel("map_server.txt"))
		{
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();
		}
	} // doStart

	//--------------------------------------------------------

	void CGameStateServer::dataPacketReceived(Net::CPaquete* packet)
	{
		Net::CBuffer buffer(packet->getDataLength());
		buffer.write(packet->getData(), packet->getDataLength());
		// Desplazamos el puntero al principio para realizar la lectura
		buffer.reset();

		Net::NetMessageType msg;
		//memcpy(&msg, packet->getData(),sizeof(msg));
		buffer.read(&msg, sizeof(msg));
		
		switch (msg)
		{
		case Net::MAP_LOADED:
		{

			// @todo Borrar las siguientes 3 líneas. Están de momento para que
			// se lance el estado de juego al cargar el mapa

			//Almacenamos el ID del usuario que se ha cargado el mapa.
			_mapLoadedByClients.push_back(packet->getConexion()->getId());

			//Si todos los clientes han cargado los mapas pasamos a crear jugadores.
			if(_clients.size() == _mapLoadedByClients.size())
			{
				// Se debe crear un jugador por cada cliente registrado.
				for(TNetIDList::const_iterator it = _clients.begin(); it != _clients.end(); it++)
				{
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
				_app->setState("game");
			}
			break;
		}
		}

	} // dataPacketReceived

	//--------------------------------------------------------

	void CGameStateServer::connexionPacketReceived(Net::CPaquete* packet)
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
		}

	} // connexionPacketReceived

	//--------------------------------------------------------

	void CGameStateServer::disconnexionPacketReceived(Net::CPaquete* packet)
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
	*/
} // namespace Application
