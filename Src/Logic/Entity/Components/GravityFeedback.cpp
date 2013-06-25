/**
@file Gravity.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CGravityFeedback
@see Logic::ISpellFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "GravityFeedback.h"
#include "HudWeapons.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Logic/Messages/MessageChangeGravity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CGravityFeedback);

	//__________________________________________________________________

	CGravityFeedback::CGravityFeedback() : ISpellFeedback("gravity"), _newGravity(0), _defaultGravity(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CGravityFeedback::~CGravityFeedback() {
		// Nada que hacer
	}


	//__________________________________________________________________

	bool CGravityFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellFeedback::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute(_spellName + "newGravity") && "Error: No se ha definido el atributo gravity new gravity en el mapa" );
		_newGravity = entityInfo->getFloatAttribute(_spellName + "newGravity");

		assert( entityInfo->hasAttribute("gravity") && "Error: No se ha definido el atributo gravity en el mapa" );
		_defaultGravity = entityInfo->getFloatAttribute("gravity");

		assert(_newGravity != 0);
		return true;
	} // spawn
	//__________________________________________________________________

	void CGravityFeedback::spell(){
		// Efecto cuando se activa la gravedad
		auto msg =  std::make_shared<CMessageChangeGravity>(_newGravity);
		_entity->emitMessage(msg);

	} // spell
	//__________________________________________________________________
		
	void CGravityFeedback::stopSpell(){
		// Efecto cuando se desactiva la gravedad
		auto msg = std::make_shared<CMessageChangeGravity>(_defaultGravity);
		_entity->emitMessage(msg);	

	} // stopSpell
	//__________________________________________________________________

}//namespace Logic