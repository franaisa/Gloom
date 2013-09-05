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
#include "TDMServer.h"

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
		
		// Cogemos el mapa en el que crear la partida y lo cargamos
		_map=args.at(0).getString();

		// Inicializar dispatcher - 0 es el id del server
		Logic::CEntityFactory::getSingletonPtr()->initDispatcher(0, args.at(4).getNumber());

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

		std::string mode = args.at(1).getString();
		if(mode == "Duel") {
			_app->setState("DMServer");
			Application::CDMServer* state = static_cast<CDMServer*>( _app->getNextState() );
			std::vector<std::string> mapList;
			mapList.push_back(_map);

			state->serverSettings(args.at(2).getString(), args.at(3).getString(), 2, args.at(5).getNumber(), false, false);
			state->gameSettings(mapList, false, std::pair<unsigned int, unsigned int>(args.at(7).getNumber(), 0), args.at(8).getNumber(), args.at(6).getBool(), args.at(11).getBool());
		}
		else if (mode == "DeathMatch") {
			_app->setState("DMServer");
			Application::CDMServer* state = static_cast<CDMServer*>( _app->getNextState() );
			std::vector<std::string> mapList;
			mapList.push_back(_map);

			state->serverSettings(args.at(2).getString(), args.at(3).getString(), args.at(4).getNumber(), args.at(5).getNumber(), false, false);
			state->gameSettings(mapList, false, std::pair<unsigned int, unsigned int>(args.at(7).getNumber(), 0), args.at(8).getNumber(), args.at(6).getBool(), args.at(11).getBool());
		}
		else if (mode == "Team DeathMatch") {
			_app->setState("TDMServer");
			Application::CTDMServer* state = static_cast<CTDMServer*>( _app->getNextState() );
			std::vector<std::string> mapList;
			mapList.push_back(_map);

			state->serverSettings(args.at(2).getString(), args.at(3).getString(), args.at(4).getNumber(), args.at(5).getNumber(), false, false);
			state->gameSettings(mapList, false, std::pair<unsigned int, unsigned int>(args.at(7).getNumber(), 0), args.at(8).getNumber(), args.at(6).getBool(), args.at(11).getBool(), true);
		}

		//state->gameSettings(mapList, false, std::pair<unsigned int, unsigned int>(15, 0), 5, false, false, true);
		/*
					0			manager.mapas.actualMap, 
					1			manager.modo.actualMode, 
					2			manager.settings.servername.text,
					3			manager.settings.serverpass.text,
					4			int(manager.settings.players.text),
					5			int(manager.settings.spectators.text),
					6			manager.gamerules.forcerespawn,
					7			int(manager.gamerules.matchtime.text),
					8			int(manager.gamerules.fraglimit.text),
					9			manager.settings.autobalance.selected,
					10			manager.gamerules.ff.selected,
					11			manager.gamerules.warmup.selected);
		*/
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
