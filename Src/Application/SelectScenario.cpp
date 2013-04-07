//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llansó
@date Agosto, 2010
*/

#include "SelectScenario.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <direct.h>

#include "GUI\Server.h"

namespace Application {

	CSelectScenario::~CSelectScenario() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CSelectScenario::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_menu = GUI::CServer::getSingletonPtr()->addLayoutToState(this,"singleplayer", Hikari::Position(Hikari::Center));
		_menu->load("SinglePlayer.swf");
		_menu->bind("back",Hikari::FlashDelegate(this, &CSelectScenario::backReleased));
		_menu->bind("createGame",Hikari::FlashDelegate(this, &CSelectScenario::loadScenario));
		_menu->hide();
		listFiles();
		return true;

	} // init

	//--------------------------------------------------------

	void CSelectScenario::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CSelectScenario::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		_menu->show();
	} // activate

	//--------------------------------------------------------

	void CSelectScenario::deactivate() 
	{		
		CApplicationState::deactivate();
		// Desactivamos la ventana GUI con el menú y el ratón.
		_menu->hide();

	} // deactivate

	//--------------------------------------------------------

	void CSelectScenario::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CSelectScenario::keyPressed(Input::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CSelectScenario::keyReleased(Input::TKey key)
	{
		switch(key.keyId)
		{
		//case Input::Key::NUMBER1:
			//return loadScenario("1");
			//_app->setState("lobbyserver");
			//break;
		//case Input::Key::NUMBER2:
			//return loadScenario("2");
			//_app->setState("lobbyclient");
			//break;
		default:
			return true;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CSelectScenario::mouseMoved(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CSelectScenario::mousePressed(const Input::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CSelectScenario::mouseReleased(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			

	//--------------------------------------------------------

	Hikari::FlashValue CSelectScenario::loadScenario(Hikari::FlashControl* caller, const Hikari::Arguments& args){
	
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;
		if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt")) {
			return false;
		}
		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel(args.at(0).getString()+".txt"))
			return false;
		
		_app->setState("singlePlayer");
		return true;
	}


	Hikari::FlashValue CSelectScenario::backReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		_app->setState("menu");
		return true;

	} // backReleased


	void CSelectScenario::listFiles(){
		WIN32_FIND_DATA FindData;
		HANDLE hFind;
		hFind = FindFirstFile("media\\maps\\*.txt", &FindData);

		std::string filename;
		while (FindNextFile(hFind, &FindData))
		{     
			filename = FindData.cFileName;
			if(filename == "map2.txt" || filename == "map1.txt")
				_menu->callFunction("pushFile",Hikari::Args(filename.substr(0,filename.find(".txt"))));

		}
	}

} // namespace Application
