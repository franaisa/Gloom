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

#include "GUI/Server.h"
#include "GUI/GUIEventArgs.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CMenuState::init() 
	{
		CApplicationState::init();

		// En el propio estado para cambiar a otra configuracion
		//GUI::getSingletonPtr()->changeStateLayout(this, "layout que nos interese");

		//CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Menu.layout");
		//_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("Menu");

		GUI::CServer::getSingletonPtr()->addLayoutToState(this, "Menu");

		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Start")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::startReleased, this));

		//GUI::CServer::getSingletonPtr()->addButtonToLayout(this, "Menu", "Start", &CMenuState::startReleased);
		
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Exit")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::exitReleased, this));

		//GUI::CServer::getSingletonPtr()->addButtonToLayout(this, "Menu", "Exit", &CMenuState::exitReleased);
	
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

		GUI::CServer::getSingletonPtr()->activateGUI(this, "Menu");
		GUI::CServer::getSingletonPtr()->activateMouseCursor();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		/*CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();*/

	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{	
		GUI::CServer::getSingletonPtr()->deactivateGUI();
		GUI::CServer::getSingletonPtr()->deactivateMouseCursor();

		// Desactivamos la ventana GUI con el menú y el ratón.
		/*CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		*/
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CMenuState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->exitRequest();
			break;
		case GUI::Key::RETURN:
			_app->setState("game");
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CMenuState::startReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("game");
		return true;

	} // startReleased
			
	/*bool CMenuState::startReleased(const GUI::GUIEventArgs& e)
	{
		_app->setState("game");
		return true;

	} // startReleased*/

	//--------------------------------------------------------

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->exitRequest();
		return true;

	} // exitReleased

} // namespace Application
