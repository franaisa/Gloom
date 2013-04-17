#include "GUIManager.h"
#include "Logic/Server.h"
#include "Hikari.h"
#include "GUI/Server.h"
#include "BaseSubsystems\Server.h"
#include "GUIKillersMessage.h"

#include <cassert>

namespace Logic {

	CGUIManager* CGUIManager::_instance = 0;

	CGUIManager::CGUIManager(){
		_instance = this;
	}

	CGUIManager::~CGUIManager(){
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CGUIManager::Init(){
		assert(!_instance && "Segunda inicialización de Logic::CGUIManager no permitida!");

		new CGUIManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}


		return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::Release()
	{
		assert(_instance && "Logic::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}
	} // Release

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CGUIManager::open(){

		//Hikari::FlashControl* menuSeleccion = GUI::CServer::getSingletonPtr()->addLayout("menuseleccion", Hikari::Position(Hikari::Center), 0.8f);
		
		//menuSeleccion->hide();

		//addGUI(menuSeleccion,"menuseleccion");
		return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::close(){
		for(TGUI::const_iterator it = _loadedGUIs.begin(); it!= _loadedGUIs.end(); ++it){
			GUI::CServer::getSingletonPtr()->destroyLayout(it->second);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CGUIManager::addGUI(Hikari::FlashControl* overlay, const std::string &name){
		GUIPair elem(name,overlay);
		_loadedGUIs.insert(elem);
		return false;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::activate(){
		//Hikari::FlashControl* menuS = _loadedGUIs.find("menuseleccion")->second;
		//menuS->load("SeleccionPersonaje.swf");
		//menuS->show();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	Hikari::FlashControl* CGUIManager::getGUIControl(const std::string &name){
		return _loadedGUIs.find(name)->second;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::deleteGUI(const std::string &gui){
		TGUI::const_iterator deletedGUI = _loadedGUIs.find(gui);

		if(deletedGUI == _loadedGUIs.end())
			return;

		GUI::CServer::getSingletonPtr()->destroyLayout(deletedGUI->second);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::showGUI(const std::string &name){
		_loadedGUIs.find(name)->second->show();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::hideGUI(const std::string &name){
		_loadedGUIs.find(name)->second->hide();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<class T, typename ReturnType>
	void CGUIManager::addCallback(const std::string &gui, const std::string &funcName, T instance, ReturnType (T::*function)()){
		Hikari::FlashControl* control = _loadedGUIs.find(gui)->second;
		control->bind(funcName, Hikari::FlashDelegate(instance,T::&function));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::callFunction(const std::string &gui, const std::string &function){
		Hikari::FlashControl* control = _loadedGUIs.find(gui)->second;
		control->callFunction(function,Hikari::Args());
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::callFunction(const std::string &gui, const std::string &function, const std::vector<std::string> &stringPars, 
						const std::vector<float> &floatPars, 
						const std::vector<int> &intPars)
	{
		Hikari::FlashControl* control = _loadedGUIs.find(gui)->second;
		Hikari::Args args = Hikari::Args();

		//primero metemos los parametros de tipo string
		if(!stringPars.empty()){
			for(int i=0;i<stringPars.size();++i){
				args(stringPars[i]);
			}
		}
		//despues metemos los parametros de tipo float
		if(!intPars.empty()){
			for(int i=0;i<floatPars.size();++i){
				args(floatPars[i]);
			}
		}
		//por ultimo metemos los parametros de tipo int
		if(!intPars.empty()){
			for(int i=0;i<intPars.size();++i){
				args(intPars[i]);
			}
		}

		//por ultimo, llamamos ala funcion flash con los parametros dados
		control->callFunction(function,args);

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CGUIManager::addGUI(const std::string &name,const Hikari::Position &pos){
		GUIPair elem(name,GUI::CServer::getSingletonPtr()->addLayout("menuseleccion", Hikari::Position(Hikari::Center), 0.8f));
		_loadedGUIs.insert(elem);
	}


}//namespace Logic



