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

#include "NetMenuState.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CNetMenuState::~CNetMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CNetMenuState::init() 
	{
		CApplicationState::init();

		_menu = GUI::CServer::getSingletonPtr()->addLayoutToState(this,"netmenu", Hikari::Position(Hikari::Center));
		_menu->load("NetworkGame.swf");
		_menu->bind("multiplayer",Hikari::FlashDelegate(this, &CNetMenuState::serverReleased));
		_menu->bind("newgame",Hikari::FlashDelegate(this, &CNetMenuState::clientReleased));
		_menu->bind("back",Hikari::FlashDelegate(this, &CNetMenuState::backReleased));
		_menu->hide();
		return true;

	} // init

	//--------------------------------------------------------

	void CNetMenuState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CNetMenuState::activate() 
	{
		CApplicationState::activate();

		_menu->show();

	} // activate

	//--------------------------------------------------------

	void CNetMenuState::deactivate() 
	{		
		_menu->hide();

	} // deactivate

	//--------------------------------------------------------

	void CNetMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CNetMenuState::keyPressed(Input::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CNetMenuState::keyReleased(Input::TKey key)
	{
		switch(key.keyId)
		{
		case Input::Key::S:
			_app->setState("lobbyserver");
			break;
		case Input::Key::C:
			_app->setState("lobbyclient");
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CNetMenuState::mouseMoved(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CNetMenuState::mousePressed(const Input::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CNetMenuState::mouseReleased(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	Hikari::FlashValue CNetMenuState::serverReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		_app->setState("lobbyserver");
		
		return true;

	} // serverReleased
			
	//--------------------------------------------------------

	Hikari::FlashValue CNetMenuState::clientReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		_app->setState("lobbyclient");
		return true;

	} // clientReleased

		//--------------------------------------------------------

	Hikari::FlashValue CNetMenuState::backReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		_app->setState("menu");
		return true;

	} // backReleased

} // namespace Application
