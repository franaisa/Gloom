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

#include "MenuState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "BaseSubsystems\Server.h"

#include "GUI/Server.h"

#include "Physics/Server.h"

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CMenuState::init() 
	{
		CApplicationState::init();

		// INICIALIZACIÓN DE LA FÍSICA
		// ---------------------------

		// TODO: desactivar colisiones entre los grupos 0 y 1
		//Physics::CServer::getSingletonPtr()->setGroupCollisions(0, 1, false);

		// TODO: Crear la escena física usando el servidor de física
		Physics::CServer::getSingletonPtr()->createScene();

		_menu = GUI::CServer::getSingletonPtr()->addLayoutToState(this,"menu", Hikari::Position(Hikari::Center));
		_menu->load("MenuPrincipal.swf");
		_menu->bind("exitClick",Hikari::FlashDelegate(this, &CMenuState::onExitClick));
		_menu->bind("multiplayer",Hikari::FlashDelegate(this, &CMenuState::multiplayerReleased));
		_menu->bind("newgame",Hikari::FlashDelegate(this, &CMenuState::startReleased));
		_menu->hide();
		return true;

	} // init

	//--------------------------------------------------------

	void CMenuState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CMenuState::activate() 
	{
		CApplicationState::activate();
		_menu->show();

	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{	
		CApplicationState::deactivate();
		_menu->hide();

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);
	} // tick

	//--------------------------------------------------------

	bool CMenuState::keyPressed(Input::TKey key)
	{
		return true;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(Input::TKey key)
	{
		switch(key.keyId)
		{
		case Input::Key::ESCAPE:
			_app->exitRequest();
			break;
		case Input::Key::RETURN:
			/*
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
				return false;
			if (!Logic::CServer::getSingletonPtr()->loadLevel("map.txt"))
				return false;

			_app->setState("singlePlayer");
			/*/
			_app->setState("selectScenario");
			/* */
			break;
			
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const Input::CMouseState &mouseState)
	{
		return true;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const Input::CMouseState &mouseState)
	{
		return true;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuState::mouseReleased(const Input::CMouseState &mouseState)
	{
		return true;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	Hikari::FlashValue CMenuState::startReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args){
		
		_app->setState("selectScenario");
		
		/* */
		return FLASH_VOID;

	} // startReleased
			
	/*bool CMenuState::startReleased(const GUI::GUIEventArgs& e)
	{
		_app->setState("singlePlayer");
		return true;6

	} // startReleased*/

	//--------------------------------------------------------

	Hikari::FlashValue CMenuState::onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		_app->exitRequest();
		return FLASH_VOID;
	} // exitReleased

	Hikari::FlashValue CMenuState::multiplayerReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		_app->setState("netmenu");
		return FLASH_VOID;

	} // multiplayerReleased

} // namespace Application
