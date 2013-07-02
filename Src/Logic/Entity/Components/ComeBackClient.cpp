/**
@file ComeBackClient.cpp

Contiene la implementación del componente que representa al hechizo come back.
 
@see Logic::CComeBackClient

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "ComeBackClient.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "PhysicController.h"

//#include "Logic/Messages/MessageChangeGravity.h"

namespace Logic {
	IMP_FACTORY(CComeBackClient);

	CComeBackClient::~CComeBackClient() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CComeBackClient::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");
		/*assert( entityInfo->hasAttribute(_spellName + "newGravity") );

		_newGravity = entityInfo->getFloatAttribute(_spellName + "newGravity");

		assert( entityInfo->hasAttribute("gravity") && "Error: No se ha definido el atributo gravity en el mapa" );
		_defaultGravity = entityInfo->getFloatAttribute("gravity");

		assert(_newGravity == 0);*/
		return true;
	} // spawn
	//__________________________________________________________________

	void CComeBackClient::spell(){ 
		/*auto msg =  std::make_shared<CMessageChangeGravity>(_newGravity);
		_entity->emitMessage(msg);*/
		if(_ComeBackClientPosition == Vector3::ZERO){
			_ComeBackClientPosition = _entity->getPosition();
			_ComeBackClientOrientation = _entity->getOrientation();
		}else{
			// Si entro aqui, esque puedo volver atras (controlado en el ammo)
			// Quizas habria q controlar si hay alguien en dicha posicion, para que no haya solapamientos
			_physicController->setPhysicPosition(_ComeBackClientPosition);
			_entity->setOrientation(_ComeBackClientOrientation);
		}

	} // spell
	//__________________________________________________________________
		
	void CComeBackClient::stopSpell() { 

		_ComeBackClientPosition = Vector3::ZERO;
		_ComeBackClientOrientation = Quaternion::ZERO;
		/*auto msg = std::make_shared<CMessageChangeGravity>(_defaultGravity);
		_entity->emitMessage(msg);*/
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

