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

		//deshabilitamos el boton crear partida
		_menu->callFunction("disableCreate",Hikari::Args());
		

		//cogemos el mapa en el que crear la partida
		_map=args.at(0).getString();

		//mostramos en el gui lo que esta pasando
		//_menu->callFunction("pushCommand",Hikari::Args("Status: Server up. Waiting for clients ..."));

		//cargamos el mapa que hemos seleccionado

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

		//empezamos la partida

		_app->setState("gameServer");
		return FLASH_VOID;

	} // backReleased

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
