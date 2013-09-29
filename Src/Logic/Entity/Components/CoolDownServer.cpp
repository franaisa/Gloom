/**
@file CoolDownServer.cpp

Contiene la implementación del componente que representa al coolDown.
 
@see Logic::CCoolDownServer

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "CoolDownServer.h"
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
	IMP_FACTORY(CCoolDownServer);

	CCoolDownServer::~CCoolDownServer() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CCoolDownServer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		Map::CEntity *tempEntity = CEntityFactory::getSingletonPtr()->getInfo(_spellName);

		// Nos aseguramos de tener todos los atributos que necesitamos
		assert( tempEntity->hasAttribute("PercentageCooldown") );

		_percentage = tempEntity->getFloatAttribute("PercentageCooldown");


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

	void CCoolDownServer::spell(){ 
		
		for(auto it = _weaponryAmmo.begin(); it < _weaponryAmmo.end() ; ++it){
			(*it)->reduceCooldown(_percentage);
		}
		for(auto it = _weaponryShoot.begin(); it < _weaponryShoot.end() ; ++it){
			(*it)->reduceCooldown(_percentage);
		}

		// Aqui pongo el feedback en 3º persona

	} // spell
	//__________________________________________________________________
		
	void CCoolDownServer::stopSpell() { 
	
		for(auto it = _weaponryAmmo.begin(); it < _weaponryAmmo.end() ; ++it){
			(*it)->reduceCooldown(0);
		}
		for(auto it = _weaponryShoot.begin(); it < _weaponryShoot.end() ; ++it){
			(*it)->reduceCooldown(0);
		}

		// Aqui quito el feedback en 3º persona

	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

