/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la interfaz con el usuario (entrada de periféricos, CEGui...).

@see GUI::CServer

@author David Llansó
@date Agosto, 2010
*/

#include "Server.h"
#include "GUIEventArgs.h"

#include "GUIDescriptor.h"
#include "BaseSubsystems/Server.h"

#include <cassert>
#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <CEGUISchemeManager.h>
#include <CEGUIFontManager.h>

namespace GUI {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() : _currentWindow(NULL)
	{
		_instance = this;
	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;
		_currentWindow = NULL;

		// Dealloc the memory reserved for the tables
		StateLayoutTable::const_iterator it = _layoutTable.begin();
		std::map<std::string, GUIDescriptor*>::const_iterator it2;
		for(; it != _layoutTable.end(); ++it) {
			for(it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
				delete it2->second;
			}
		}

	} // ~CServer
	
	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de GUI::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "GUI::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{

		//_GUISystem = BaseSubsystems::CServer::getSingletonPtr()->getGUISystem();

		// Cargamos las distintas plantillas o esquemas de fichero
		// que usaremos en nuestro GUI.
		// (automáticamente cargan los archivos looknfeel e imageset)
		CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
		CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");
		CEGUI::SchemeManager::getSingleton().create("SleekSpace.scheme");
		CEGUI::SchemeManager::getSingleton().create("CrossHair.scheme");
		// Cargamos los archivos con las fuentes que usaremos.
		CEGUI::FontManager::getSingleton().create("DejaVuSans-10.font");
		CEGUI::FontManager::getSingleton().create("FairChar-30.font");
		CEGUI::FontManager::getSingleton().create("Batang-26.font");

		#ifndef NON_EXCLUSIVE_MODE_IN_WINDOW_MODE 
		// Establecemos cual será el puntero del ratón.
		CEGUI::SchemeManager::getSingleton().create("raton.scheme");
		BaseSubsystems::CServer::getSingletonPtr()->getGUISystem()->setDefaultMouseCursor("raton","miraRaton");
		#endif	

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{

	} // close

	//________________________________________________________________________

	void CServer::addLayoutToState(Application::CApplicationState* state, 
		const std::string& layoutName) {
		
		// Find the table associated with "state" where all the information 
		// about the different layout is stored. Then find the table with that 
		// layoutName and create its descriptor (which takes care of window 
		// initialization etc)
		(_layoutTable[state])[layoutName] = new GUIDescriptor(layoutName);
	} // addLayoutToState

	//________________________________________________________________________

	void CServer::addButtonToLayout( Application::CApplicationState* state, 
		const std::string& layoutName,  const std::string& buttonName, 
		bool (*buttonFunction)(const GUIEventArgs&) ) {

		
	} // addButtonToLayout

	//________________________________________________________________________

	bool CServer::activateGUI(Application::CApplicationState* state, 
		const std::string& layoutName) {
		
		// Find the layouts table associated to "state"
		StateLayoutTable::const_iterator it = _layoutTable.find(state);

		// If there is such a state in our table
		if(it != _layoutTable.end()) {
			// Find the layout given in "layoutName"
			std::map<std::string, GUIDescriptor*>::const_iterator it2 = 
				it->second.find(layoutName);

			// If there is such a layout name
			if(it2 != it->second.end()) {
				// Activate the window and update "_currentWindow" pointer
				_currentWindow = it2->second->activate();
				return true;
			}

		}

		return false;
	} // activateGUI

	//________________________________________________________________________

	void CServer::activateMouseCursor() {
		// Show mouse cursor
		CEGUI::MouseCursor::getSingleton().show();
	} // activateMouseCursor

	//________________________________________________________________________

	void CServer::deactivateGUI() {
		_currentWindow->deactivate();
		_currentWindow->setVisible(false);
		_currentWindow = NULL;
	} // deactivateGUI

	//________________________________________________________________________

	void CServer::deactivateMouseCursor() {
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
	} // deactivateMouseCursor

	//________________________________________________________________________

	void CServer::setText(const std::string& msg) {
		_currentWindow->setText(msg.c_str());
	}

} // namespace GUI
