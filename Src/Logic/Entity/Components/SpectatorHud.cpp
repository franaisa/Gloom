#include "SpectatorHud.h"

#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "Logic/Maps/GUIManager.h"

#include "FlashControl.h"

#include <assert.h>

namespace Logic{

	IMP_FACTORY(CSpectatorHud);

	CSpectatorHud::CSpectatorHud(){}

	CSpectatorHud::~CSpectatorHud(){}

	bool CSpectatorHud::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		CGUIManager * guiManager = Logic::CGUIManager::getSingletonPtr();
		guiManager->addGUI("SpectatorHud", Hikari::Position(Hikari::Center));
		guiManager->load("SpectatorHud", "SpectatorHud.swf");
		guiManager->setTransparent("SpectatorHud",true);
		_hud = guiManager->getGUIControl("SpectatorHud");
		_hud->hide();
		return true;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	void CSpectatorHud::onDeactivate(){
		_hud->hide();
	}

	void CSpectatorHud::onActivate(){
		_hud->show();
	}

	void CSpectatorHud::freelook(){
		_hud->callFunction("freelook");
	}

	void CSpectatorHud::lookAt(const std::string &name){
		_hud->callFunction("lookAt",Hikari::Args(name));
	}

}