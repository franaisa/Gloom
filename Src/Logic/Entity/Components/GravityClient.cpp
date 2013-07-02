/**
@file GravityClient.cpp

Contiene la implementación del componente que representa al gravity.
 
@see Logic::CGravityClient

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "GravityClient.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageChangeGravity.h"


namespace Logic {
	IMP_FACTORY(CGravityClient);

	CGravityClient::~CGravityClient() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CGravityClient::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpell::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute(_spellName + "newGravity") && "Error: No se ha definido el atributo gravity new gravity en el mapa" );
		_newGravity = entityInfo->getFloatAttribute(_spellName + "newGravity");

		assert( entityInfo->hasAttribute("gravity") && "Error: No se ha definido el atributo gravity en el mapa" );
		_defaultGravity = entityInfo->getFloatAttribute("gravity");

		assert(_newGravity != 0);
		return true;
	} // spawn
	//__________________________________________________________________

	void CGravityClient::spell(){
		// Efecto cuando se activa la gravedad
		auto msg =  std::make_shared<CMessageChangeGravity>(_newGravity);
		_entity->emitMessage(msg);

		// Feedback cuando se activa la gravedad

	} // spell
	//__________________________________________________________________
		
	void CGravityClient::stopSpell(){
		// Efecto cuando se desactiva la gravedad
		auto msg = std::make_shared<CMessageChangeGravity>(_defaultGravity);
		_entity->emitMessage(msg);	

		// Fin feedback cuando se activa la gravedad
	} // stopSpell
	//__________________________________________________________________

	

	
} // namespace Logic

