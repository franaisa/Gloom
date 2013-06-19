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
#include "DMServer.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"

#include "GUI/Server.h"
#include "Hikari.h"

#include <cstdlib>
#include <direct.h>

namespace Application {

	CLobbyServerState::CLobbyServerState(CBaseApplication *app) : CApplicationState(app) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CLobbyServerState::~CLobbyServerState() {
		// Nada que borrar
	}

	//__________________________________________________________________

	bool CLobbyServerState::init() {
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
	}

	//__________________________________________________________________

	void CLobbyServerState::activate() {
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		_menu->show();
	}

	//__________________________________________________________________

	void CLobbyServerState::deactivate() {	
		CApplicationState::deactivate();

		// Desactivamos el menú
		_menu->hide();
	}

	//__________________________________________________________________

	bool CLobbyServerState::keyPressed(Input::TKey key){
	   return false;
	} // keyPressed

	//__________________________________________________________________

	bool CLobbyServerState::keyReleased(Input::TKey key) {
		switch(key.keyId) {
			case Input::Key::ESCAPE: {
				_app->setState("netmenu");
				break;
			}
			default: {
				return false;
			}
		}

		return true;
	} // keyReleased

	//__________________________________________________________________
	
	bool CLobbyServerState::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//__________________________________________________________________
		
	bool CLobbyServerState::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//__________________________________________________________________


	bool CLobbyServerState::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased
			
	//__________________________________________________________________
		
	Hikari::FlashValue CLobbyServerState::startReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args) {
		return FLASH_VOID;
	} // startReleased
			
	//__________________________________________________________________

	Hikari::FlashValue CLobbyServerState::backReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args) {
		_app->setState("netmenu");
		return FLASH_VOID;
	} // backReleased

	//__________________________________________________________________

	Hikari::FlashValue CLobbyServerState::createReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args) {
		// Deshabilitamos el boton crear partida
		_menu->callFunction("disableCreate",Hikari::Args());
		
		// Cogemos el mapa en el que crear la partida y lo cargamos
		_map=args.at(0).getString();

		// Inicializar dispatcher - 0 es el id del server
		// @deprecated El numero de jugadores maximo debe leerse de flash
		Logic::CEntityFactory::getSingletonPtr()->initDispatcher(0, 12);

		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_server.txt")) {
			_app->exitRequest();
		}

		if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt")) {
			_app->exitRequest();
		}

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel(_map+"_server.txt")) {
			_app->exitRequest();
		}
		
		// Empezamos la partida en modo servidor
		_app->setState("DMServer");

		// @todo En funcion de los datos leidos de flash, establecer los parametros
		// de configuracion del servidor y de la partida
		Application::CDMServer* state = static_cast<CDMServer*>( _app->getNextState() );
		std::vector<std::string> mapList;
		mapList.push_back(_map);

		state->serverSettings("Servidor Gazpachero", "cobragay", 8, 4, false, false);
		state->gameSettings(mapList, false, std::pair<unsigned int, unsigned int>(15, 0), 1, false, false);
		//state->gameSettings(mapList, false, std::pair<unsigned int, unsigned int>(15, 0), 5, false, false, true);

		return FLASH_VOID;
	} // backReleased

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLobbyServerState::listFiles() {
		WIN32_FIND_DATA FindData;
		HANDLE hFind;
		hFind = FindFirstFile("media\\maps\\*.map", &FindData);

		std::string filename;

		if(hFind){
			filename = FindData.cFileName;
			_menu->callFunction("pushFile",Hikari::Args(filename.substr(0,filename.find(".map"))));
		}


		while (FindNextFile(hFind, &FindData))
		{     
			filename = FindData.cFileName;
			_menu->callFunction("pushFile",Hikari::Args(filename.substr(0,filename.find(".map"))));

		}
	}

} // namespace Application
