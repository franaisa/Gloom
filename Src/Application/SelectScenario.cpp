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

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CSelectScenario::~CSelectScenario() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CSelectScenario::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("SelectScenario.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("SelectScenario");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		
		CEGUI::WindowManager::getSingleton().getWindow("SelectScenario/Scenario1")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CSelectScenario::scenario1Released, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("SelectScenario/Scenario2")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CSelectScenario::scenario2Released, this));

		CEGUI::WindowManager::getSingleton().getWindow("SelectScenario/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CSelectScenario::backReleased, this));

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
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

	} // activate

	//--------------------------------------------------------

	void CSelectScenario::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

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
		case Input::Key::NUMBER1:
			return loadScenario("1");
			//_app->setState("lobbyserver");
			break;
		case Input::Key::NUMBER2:
			return loadScenario("2");
			//_app->setState("lobbyclient");
			break;
		default:
			return false;
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
		
	bool CSelectScenario::scenario1Released(const CEGUI::EventArgs& e)
	{
		return loadScenario("1");

	} // serverReleased
			
	//--------------------------------------------------------

	bool CSelectScenario::scenario2Released(const CEGUI::EventArgs& e)
	{
		return loadScenario("2");

	} // clientReleased

		//--------------------------------------------------------

	bool CSelectScenario::loadScenario(const std::string &name){
	
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;
		if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt")) {
			return false;
		}
		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel("map"+name+".txt"))
			return false;
		
		_app->setState("singlePlayer");
		return true;
	}


	bool CSelectScenario::backReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("menu");
		return true;

	} // backReleased

} // namespace Application
