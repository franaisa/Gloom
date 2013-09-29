/**
@file AmplifyDamageServer.cpp

Contiene la implementación del componente que representa al hechizo de amplificar daño.
 
@see Logic::CAmplifyDamageServer

@author Jaime Chapinal Cervantes
@date Junio,2013
*/

#include "AmplifyDamageServer.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"


#include "ShotGun.h"
#include "Sniper.h"
#include "MiniGun.h"
#include "SoulReaper.h"
#include "IronHellGoat.h"


#include "Logic/Messages/MessageDamageAmplifier.h"

namespace Logic {
	IMP_FACTORY(CAmplifyDamageServer);

	CAmplifyDamageServer::~CAmplifyDamageServer() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CAmplifyDamageServer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		Map::CEntity *tempEntity = CEntityFactory::getSingletonPtr()->getInfo(_spellName);

		assert( tempEntity ->hasAttribute("PercentDamage") );

		_newPercentDamage = tempEntity ->getIntAttribute("PercentDamage");

		assert(_newPercentDamage != 0);

		_weaponry.resize(WeaponType::eSIZE);
		// Rellenamos el vector con los punteros a los componentes correspondientes
		_weaponry[WeaponType::eSOUL_REAPER] = _entity->getComponent<Logic::CSoulReaper>("CSoulReaper");
		_weaponry[WeaponType::eSNIPER]= _entity->getComponent<Logic::CSniper>("CSniper");
		_weaponry[WeaponType::eSHOTGUN]= _entity->getComponent<Logic::CShotGun>("CShotGun");
		_weaponry[WeaponType::eMINIGUN]= _entity->getComponent<Logic::CMiniGun>("CMiniGun");
		_weaponry[WeaponType::eIRON_HELL_GOAT]= _entity->getComponent<Logic::CIronHellGoat>("CIronHellGoat");

		return true;
	} // spawn
	//__________________________________________________________________

	void CAmplifyDamageServer::spell(){ 
		/*
		auto msg =  std::make_shared<CMessageDamageAmplifier>(_newPercentDamage);
		_entity->emitMessage(msg);
		*/
		for(unsigned int i = 0; i < WeaponType::eSIZE;++i){
			_weaponry[i]->amplifyDamage(_newPercentDamage);
			
		}

		// Aqui pongo el feedback en 3º persona

	} // spell
	//__________________________________________________________________
		
	void CAmplifyDamageServer::stopSpell() {
		/*
		auto msg = std::make_shared<CMessageDamageAmplifier>(0);
		_entity->emitMessage(msg);
		*/
		for(unsigned int i = 0; i < WeaponType::eSIZE;++i){
			_weaponry[i]->amplifyDamage(0);
		}

		// Aqui quito el feedback en 3º persona

	} // stopSpell
	//__________________________________________________________________

} // namespace Logic

