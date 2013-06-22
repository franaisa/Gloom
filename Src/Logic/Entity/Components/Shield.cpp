/**
@file Shield.cpp

Contiene la implementación del componente que representa al hechizo shield.
 
@see Logic::CShield

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "Shield.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"


#include "ShieldSpellController.h"
#include "Logic/Messages/MessageSetReducedDamage.h"

namespace Logic {
	IMP_FACTORY(CShield);

	CShield::~CShield() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CShield::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		assert( entityInfo->hasAttribute("heightShoot") );
		assert( entityInfo->hasAttribute(_spellName + "Damage") );
		assert( entityInfo->hasAttribute(_spellName + "PercentageAbsorbed") );

		_heightShoot = entityInfo->getFloatAttribute("heightShoot");
		_damage = entityInfo->getFloatAttribute(_spellName + "Damage");
		_percentageAbsorbed = entityInfo->getFloatAttribute(_spellName + "PercentageAbsorbed");
	
		/*assert( entityInfo->hasAttribute(_spellName + "newGravity") );

		_newGravity = entityInfo->getFloatAttribute(_spellName + "newGravity");

		assert( entityInfo->hasAttribute("gravity") && "Error: No se ha definido el atributo gravity en el mapa" );
		_defaultGravity = entityInfo->getFloatAttribute("gravity");

		assert(_newGravity == 0);*/
		return true;
	} // spawn
	//__________________________________________________________________

	void CShield::spell(){ 

		// Quizas seria mejor, crear la entidad en cuanto se coge el hechizo y tan solo activarlo y desactivarlo cuando se use.

		if(_shieldSpell){
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_shieldSpell,true);
			_shieldSpell = NULL;
		}else{
			// Creo el escudo, el cual se encargara de hacer daño a quien entre dentro del radio, especificado en la entidad "ShieldSpell"
			_shieldSpell = CEntityFactory::getSingletonPtr()->createEntity(
				CEntityFactory::getSingletonPtr()->getInfo("ShieldSpell"),			
				Logic::CServer::getSingletonPtr()->getMap(),
				_entity->getPosition()+Vector3(0,_heightShoot*0.5,0),
				Quaternion::IDENTITY
			);
			_shieldSpell->getComponent<CShieldSpellController>("CShieldSpellController")->setOwner(_entity);
			_shieldSpell->getComponent<CShieldSpellController>("CShieldSpellController")->setProperties(_damage);
			_shieldSpell->activate();
			_shieldSpell->start();

			// Indico que reduzca un pelin el daño entrante
			// La pega de hacerlo asi, es que solo proteje al emisor y no a quien se pusiera tras el escudo.
			std::shared_ptr<CMessageSetReducedDamage> pReducedDmgMsg = std::make_shared<CMessageSetReducedDamage>();
			pReducedDmgMsg->setReducedDamage(_percentageAbsorbed);
			_entity->emitMessage(pReducedDmgMsg);	
		}

	} // spell
	//__________________________________________________________________
		
	void CShield::stopSpell() {
		if(_shieldSpell){
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_shieldSpell,true);
			_shieldSpell = NULL;
		}
		/*auto msg = std::make_shared<CMessageChangeGravity>(_defaultGravity);
		_entity->emitMessage(msg);*/
	} // stopSpell
	//__________________________________________________________________

	void CShield::onDeactivate(){
		if(_shieldSpell){
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_shieldSpell,true);
			_shieldSpell = NULL;
		}
	}
} // namespace Logic

