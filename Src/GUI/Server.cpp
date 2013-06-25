/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la interfaz con el usuario (entrada de periféricos, CEGui...).

@see GUI::CServer

@author David Llansó
@author Rubén Mulero
@date August, 2010
*/

#include "Server.h"
#include "BaseSubsystems/Server.h"

#include <cassert>
#include "Hikari.h"
#include "FlashControl.h"
#include "FlashValue.h"
#include "HikariPlatform.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

namespace GUI {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer()
	{
		_instance = this;
	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;

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
		_manager = BaseSubsystems::CServer::getSingletonPtr()->getHikari();
		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{

	} // close

	//________________________________________________________________________

	Hikari::FlashControl* CServer::addLayout(const std::string& layoutName, Hikari::Position pos, unsigned int width, unsigned int height, unsigned short zOrder) {

			return BaseSubsystems::CServer::getSingletonPtr()->getHikari()->
							createFlashOverlay(layoutName,Graphics::CServer::getSingletonPtr()->getActiveScene()->getViewport(),
							width, height, pos, zOrder);
	}

	//________________________________________________________________________

	Hikari::FlashControl* CServer::addLayout(const std::string& layoutName, Hikari::Position pos, float relativePos, unsigned short zOrder) {

			return BaseSubsystems::CServer::getSingletonPtr()->getHikari()->
							createFlashOverlay(layoutName,Graphics::CServer::getSingletonPtr()->getActiveScene()->getViewport(),
							BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getWidth()*relativePos, 
							BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getHeight()*relativePos, 
							pos, zOrder);
	}

	//________________________________________________________________________

	Hikari::FlashControl* CServer::addLayout(const std::string& layoutName, Hikari::Position pos, unsigned short zOrder) {

			return BaseSubsystems::CServer::getSingletonPtr()->getHikari()->
							createFlashOverlay(layoutName,Graphics::CServer::getSingletonPtr()->getActiveScene()->getViewport(),
							BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getWidth(), 
							BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getHeight(), 
							pos, zOrder);
	}

	//________________________________________________________________________


	Hikari::FlashControl* CServer::addLayoutToState(Application::CApplicationState* state, 
		const std::string& layoutName, Hikari::Position pos) {

			return BaseSubsystems::CServer::getSingletonPtr()->getHikari()->
							createFlashOverlay(layoutName,Graphics::CServer::getSingletonPtr()->getActiveScene()->getViewport(),
							BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getWidth(), 
							BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getHeight(), pos,1);
	}

	//________________________________________________________________________

	void CServer::destroyLayout(Hikari::FlashControl* overlay){
		_manager->destroyFlashControl(overlay);
	}

	//________________________________________________________________________

	void CServer::addButtonToLayout( Application::CApplicationState* state, 
		const std::string& layoutName,  const std::string& buttonName, 
		bool (*buttonFunction)(const GUIEventArgs&) ) {

		
	} // addButtonToLayout

	//________________________________________________________________________

	bool CServer::activateGUI(Application::CApplicationState* state, 
		const std::string& layoutName) {

		return true;
	} // activateGUI

	void CServer::tick(){
		_manager->update();
	}

} // namespace GUI
