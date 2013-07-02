/**
@file AmplifyDamageClient.cpp

Contiene la implementación del componente que representa al hechizo de amplificar daño.
 
@see Logic::CAmplifyDamageClient

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "AmplifyDamageClient.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageDamageAmplifier.h"

namespace Logic {
	IMP_FACTORY(CAmplifyDamageClient);

	CAmplifyDamageClient::~CAmplifyDamageClient() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CAmplifyDamageClient::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		return true;
	} // spawn
	//__________________________________________________________________

	void CAmplifyDamageClient::spell(){ 
		// Feedback de que tengo el daño ampliado

	} // spell
	//__________________________________________________________________
		
	void CAmplifyDamageClient::stopSpell() {
		// Fin feedback de que tengo el daño ampliado

	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

