/**
@file CoolDown.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownFeedback
@see Logic::ISpellFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "ZoomFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"


#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CZoomFeedback);

	//__________________________________________________________________

	CZoomFeedback::CZoomFeedback() : ISpellFeedback("zoom"), _camera(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CZoomFeedback::~CZoomFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CZoomFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		_camera = _entity->getMap()->getScene()->getCamera();

		return true;
	} // spawn
	//__________________________________________________________________

	void CZoomFeedback::spell(){
		_camera->setFov(Math::PI/8);

	} // spell
	//__________________________________________________________________
		
	void CZoomFeedback::stopSpell(){
		_camera->resetFov();

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic