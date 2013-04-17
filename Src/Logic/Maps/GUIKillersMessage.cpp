#include "GUIKillersMessage.h"
#include <cassert>
#include <vector>
#include "Hikari.h"

namespace Logic{
	GUIKillersMessage* GUIKillersMessage::_instance = 0;

	GUIKillersMessage::GUIKillersMessage(){
		assert(!_instance && "Segunda inicialización de Logic::CGUIKillersMessage no permitida!");

		_instance = this;

		_guiManager = CGUIManager::getSingletonPtr();
		_guiManager->addGUI("killerbox", Hikari::Position(Hikari::TopRight));
	}

	void GUIKillersMessage::addKiller(const std::string &killer, const std::string &killed){
		std::vector<std::string> strParams;
		strParams.push_back(killer);
		strParams.push_back(killed);
		std::vector<int> intParams;
		std::vector<float> floatParams;
		_guiManager->callFunction("killerbox", "addKillers", strParams,floatParams,  intParams);
	}
}