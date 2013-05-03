//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementaci�n del estado de men�.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llans�
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

		// Cargamos la ventana que muestra el men�
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

		// Activamos la ventana que nos muestra el men� y activamos el rat�n.
		_menu->show();
	} // activate

	//--------------------------------------------------------

	void CSelectScenario::deactivate() 
	{		
		CApplicationState::deactivate();
		// Desactivamos la ventana GUI con el men� y el rat�n.
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
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CSelectScenario::mouseMoved(const Input::CMouseState &mouseState)
	{
		return true;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CSelectScenario::mousePressed(const Input::CMouseState &mouseState)
	{
		return true;

	} // mousePressed

	//--------------------------------------------------------


	bool CSelectScenario::mouseReleased(const Input::CMouseState &mouseState)
	{
		return true;

	} // mouseReleased
			

	//--------------------------------------------------------

	Hikari::FlashValue CSelectScenario::loadScenario(Hikari::FlashControl* caller, const Hikari::Arguments& args){
		// Inicializar dispatcher para SP
		Logic::CEntityFactory::getSingletonPtr()->initDispatcher();

		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;
		if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt"))
			return false;
		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel(args.at(0).getString()+".txt"))
			return false;
		
		_app->setState("singlePlayer");
		return true;
	}


	Hikari::FlashValue CSelectScenario::backReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		_app->setState("menu");
		return FLASH_VOID;

	} // backReleased


	void CSelectScenario::listFiles(){
		WIN32_FIND_DATA FindData;
		HANDLE hFind;
		hFind = FindFirstFile("media\\maps\\*.txt", &FindData);

		std::string filename;
		while (FindNextFile(hFind, &FindData))
		{     
			filename = FindData.cFileName;
			if(filename == "map2.txt" || filename == "map1.txt" || filename == "scenarioTest.txt")
				_menu->callFunction("pushFile",Hikari::Args(filename.substr(0,filename.find(".txt"))));

		}
	}

} // namespace Application
