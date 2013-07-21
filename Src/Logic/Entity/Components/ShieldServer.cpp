/**
@file ShieldServer.cpp

Contiene la implementación del componente que representa al hechizo shield.
 
@see Logic::CShieldServer

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "ShieldServer.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"


#include "ShieldSpellController.h"
#include "Logic/Messages/MessageSetReducedDamage.h"
#include "Logic/Messages/MessageSetOwner.h"

namespace Logic {
	IMP_FACTORY(CShieldServer);

	CShieldServer::~CShieldServer() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CShieldServer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		assert( entityInfo->hasAttribute("heightShoot") );
		assert( entityInfo->hasAttribute(_spellName + "Damage") );
		assert( entityInfo->hasAttribute(_spellName + "PercentageAbsorbed") );

		_heightShoot = entityInfo->getFloatAttribute("heightShoot");
		_damage = entityInfo->getFloatAttribute(_spellName + "Damage");
		_percentageAbsorbed = entityInfo->getFloatAttribute(_spellName + "PercentageAbsorbed");
	
		return true;
	} // spawn
	//__________________________________________________________________

	void CShieldServer::spell(){ 

		// Creo el escudo, el cual se encargara de hacer daño a quien entre dentro del radio, especificado en la entidad "ShieldServerSpell"
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

		
		std::shared_ptr<CMessageSetOwner> setOwnerMsg = std::make_shared<CMessageSetOwner>();
		setOwnerMsg->setOwner(_entity);
		_shieldSpell->emitMessage(setOwnerMsg);

		// Indico que reduzca un pelin el daño entrante
		// La pega de hacerlo asi, es que solo proteje al emisor y no a quien se pusiera tras el escudo.
		std::shared_ptr<CMessageSetReducedDamage> pReducedDmgMsg = std::make_shared<CMessageSetReducedDamage>();
		pReducedDmgMsg->setReducedDamage(_percentageAbsorbed);
		_entity->emitMessage(pReducedDmgMsg);	

	} // spell
	//__________________________________________________________________
		
	void CShieldServer::stopSpell() {
		if(_shieldSpell){
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_shieldSpell,true);
			_shieldSpell = NULL;
		}
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

