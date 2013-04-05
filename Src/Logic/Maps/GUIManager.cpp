#include "GUIManager.h"
#include "Logic/Server.h"
#include "Hikari.h"
#include "GUI/Server.h"
#include "BaseSubsystems\Server.h"

#include <cassert>

namespace Logic {

	CGUIManager* CGUIManager::_instance = 0;

	CGUIManager::CGUIManager(){
		_instance = this;
	}

	CGUIManager::~CGUIManager(){
	}

	bool CGUIManager::Init(){
		assert(!_instance && "Segunda inicialización de Logic::CServer no permitida!");

		new CGUIManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}
		return true;
	}

	void CGUIManager::Release()
	{
		assert(_instance && "Logic::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}
	} // Release

	bool CGUIManager::open(){

		Hikari::FlashControl* menuSeleccion = GUI::CServer::getSingletonPtr()->addLayout("menuseleccion", Hikari::Position(Hikari::Center), 0.8f);
		
		menuSeleccion->hide();

		addGUI(menuSeleccion,"menuseleccion");
		return true;
	}

	void CGUIManager::close(){
	}

	bool CGUIManager::addGUI(Hikari::FlashControl* overlay, std::string name){
		GUIPair elem(name,overlay);
		_loadedGUIs.insert(elem);
		return false;
	}

	void CGUIManager::activate(){
		Hikari::FlashControl* menuS = _loadedGUIs.find("menuseleccion")->second;
		menuS->load("SeleccionPersonaje.swf");
	}

}



