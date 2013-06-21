/**
@file CoolDown.cpp

Contiene la implementación del componente que representa al coolDown.
 
@see Logic::CCoolDown

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "CoolDown.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"


#include "ShotGunAmmo.h"
#include "SniperAmmo.h"
#include "MiniGunAmmo.h"
#include "SoulReaperAmmo.h"
#include "IronHellGoatAmmo.h"

#include "MiniGun.h"
#include "IronHellGoat.h"

#include "Logic/Messages/MessageReducedCooldown.h"

namespace Logic {
	IMP_FACTORY(CCoolDown);

	CCoolDown::~CCoolDown() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CCoolDown::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( entityInfo->hasAttribute(_spellName + "PercentageCooldown") );

		_percentage = entityInfo->getFloatAttribute(_spellName + "PercentageCooldown");


		_weaponryAmmo.resize(WeaponType::eSIZE);
		// Rellenamos el vector con los punteros a los componentes correspondientes
		_weaponryAmmo[WeaponType::eSOUL_REAPER] = _entity->getComponent<Logic::CSoulReaperAmmo>("CSoulReaperAmmo");
		_weaponryAmmo[WeaponType::eSNIPER]= _entity->getComponent<Logic::CSniperAmmo>("CSniperAmmo");
		_weaponryAmmo[WeaponType::eSHOTGUN]= _entity->getComponent<Logic::CShotGunAmmo>("CShotGunAmmo");
		_weaponryAmmo[WeaponType::eMINIGUN]= _entity->getComponent<Logic::CMiniGunAmmo>("CMiniGunAmmo");
		_weaponryAmmo[WeaponType::eIRON_HELL_GOAT]= _entity->getComponent<Logic::CIronHellGoatAmmo>("CIronHellGoatAmmo");

		_weaponryShoot.push_back( _entity->getComponent<Logic::CIronHellGoat>("CIronHellGoat"));
		_weaponryShoot.push_back( _entity->getComponent<Logic::CMiniGun>("CMiniGun"));

		return true;
	} // spawn
	//__________________________________________________________________

	void CCoolDown::spell(){ 
		/*
		auto msg =  std::make_shared<CMessageReducedCooldown>(_percentage);
		_entity->emitMessage(msg);
		*/
		for(unsigned int i = 0; i < WeaponType::eSIZE;++i){
			_weaponryAmmo[i]->reduceCooldown(_percentage);
		}
		for(auto it = _weaponryShoot.begin(); it < _weaponryShoot.end() ; ++it){
			(*it)->reduceCooldown(_percentage);
		}

	} // spell
	//__________________________________________________________________
		
	void CCoolDown::stopSpell() { 
		/*
		auto msg =  std::make_shared<CMessageReducedCooldown>(0);
		_entity->emitMessage(msg);	
		*/
		for(unsigned int i = 0; WeaponType::eSIZE;++i){
			_weaponryAmmo[i]->reduceCooldown(0);
		}
		for(auto it = _weaponryShoot.begin(); it < _weaponryShoot.end() ; ++it){
			(*it)->reduceCooldown(0);
		}
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

