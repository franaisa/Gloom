/**
@file CoolDown.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownFeedback
@see Logic::ISpellFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "ScopeFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"


#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CScopeFeedback);

	//__________________________________________________________________

	CScopeFeedback::CScopeFeedback() : ISpellFeedback("scope"), _camera(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CScopeFeedback::~CScopeFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CScopeFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		_camera = _entity->getMap()->getScene()->getCamera();

		return true;
	} // spawn
	//__________________________________________________________________

	void CScopeFeedback::onActivate(){
		_camera->resetFov();
	} // onActivate
	//__________________________________________________________________

	void CScopeFeedback::spell(){
		_camera->setFov(Math::PI/8);

	} // spell
	//__________________________________________________________________
		
	void CScopeFeedback::stopSpell(){
		_camera->resetFov();

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic