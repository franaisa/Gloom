#include "GUIKillersMessage.h"
#include <cassert>
#include <vector>
#include "Hikari.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include "Graphics/Scene.h"

namespace Logic{
	GUIKillersMessage* GUIKillersMessage::_instance = 0;

	GUIKillersMessage::GUIKillersMessage(){
		assert(!_instance && "Segunda inicialización de Logic::CGUIKillersMessage no permitida!");

		_instance = this;

		_guiManager = CGUIManager::getSingletonPtr();
		_guiManager->addGUI("killerbox", Hikari::Position(Hikari::TopRight), 450, 300);
		_guiManager->load("killerbox", "KilledBox.swf");
		_guiManager->showGUI("killerbox");
		_guiManager->setTransparent("killerbox",true);
	}

	void GUIKillersMessage::Init(){
		assert(!_instance && "Segunda inicialización de Logic::GUIKillersMessage no permitida!");

		new GUIKillersMessage();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GUIKillersMessage::Release()
	{
		if(_instance)
		{
			delete _instance;
		}
	} // Release

	void GUIKillersMessage::addKiller(const std::string &killer, const std::string &killed){
		std::vector<std::string> strParams;
		strParams.push_back(killer);
		strParams.push_back(killed);
		std::vector<int> intParams;
		std::vector<float> floatParams;
		_guiManager->callFunction("killerbox", "addKiller", strParams,floatParams,  intParams);
	}

	void GUIKillersMessage::suicide(const std::string &name){
		std::vector<std::string> strParams;
		strParams.push_back(name);
		strParams.push_back("himself :)");
		std::vector<int> intParams;
		std::vector<float> floatParams;
		_guiManager->callFunction("killerbox", "addKiller", strParams,floatParams,  intParams);
	}

}