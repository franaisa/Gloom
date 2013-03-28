/**
@file Shoot.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShoot
@see Logic::IComponent

*/

#include "WeaponsManager.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"

#include "Logic/Entity/Components/ShootShotGun.h"
#include "Logic/Entity/Components/ShootSniper.h"
#include "Logic/Entity/Components/ShootMiniGun.h"
#include "Logic/Entity/Components/ShootHammer.h"
#include "Logic/Entity/Components/ShootGrenadeLauncher.h"
#include "Logic/Entity/Components/ShootRocketLauncher.h"

#include "Logic/Messages/MessageChangeWeapon.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"

#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageHudAmmo.h"

#include "Logic/Messages/MessageBerserker.h"


namespace Logic 
{
	IMP_FACTORY(CWeaponsManager);
	
	//---------------------------------------------------------

	bool CWeaponsManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("numWeapons")){
			_numWeapons = entityInfo->getIntAttribute("numWeapons");

			_weapons = new bool[_numWeapons];
			for(int i = 1; i<_numWeapons;++i){
				_weapons[i] = false;
				deactivateComponent(i);
			}

			/*la 1º arama siempre estara a true*/
			_weapons[0] = true;
			activateComponent(_actualWeapon);

		}
		return true;

	} // spawn
	//---------------------------------------------------------

	void CWeaponsManager::activate()
	{
		IComponent::activate();

		for(int i = 0; i<_numWeapons;++i){
			deactivateComponent(i);
		}

		/*la 1º arma siempre estara a true*/
		activateComponent(_actualWeapon);
		equippedWeapon(_actualWeapon, true);
		_weapons[_actualWeapon]=true;


		//return true;

	} // activate
	//---------------------------------------------------------


	void CWeaponsManager::deactivate(){
		//reset de armas
		for(int i = 1; i<_numWeapons;++i){
			_weapons[i] = false;
		}
		_actualWeapon=0;
		//reset de balas
		for(int i = 1; i<_numWeapons;++i){
			resetAmmo(i);
		}

		// desactivamos todas las armas excepto el hammer
		// y seteamos inUse a false en todas menos en el hammer
	
	}// deactivate
	//---------------------------------------------------------


	bool CWeaponsManager::accept(CMessage *message)
	{
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::CHANGE_WEAPON 
			|| msgType == Message::ADD_AMMO
			|| msgType == Message::ADD_WEAPON
			|| msgType == Message::BERSERKER;
	} // accept
	//---------------------------------------------------------


	void CWeaponsManager::process(CMessage* message) {
		switch( message->getMessageType() ) {
			case Message::CHANGE_WEAPON: {
				changeWeapon( static_cast<CMessageChangeWeapon*>(message)->getWeapon() );
				break;
			}
			case Message::ADD_AMMO: {
				CMessageAddAmmo* addAmmoMsg = static_cast<CMessageAddAmmo*>(message);
				addAmmo( addAmmoMsg->getAddAmmo(), addAmmoMsg->getAddWeapon() );
				break;
			}
			case Message::ADD_WEAPON: {
				CMessageAddWeapon* addWeaponMsg = static_cast<CMessageAddWeapon*>(message);
				addWeapon( addWeaponMsg->getAddAmmo(), addWeaponMsg->getAddWeapon() );
				break;
			}
			case Message::BERSERKER: {
				CMessageBerserker* berserkerMsg = static_cast<CMessageBerserker*>(message);
				amplifyDamage( berserkerMsg->getPercentDamage() );
				reduceCooldowns( berserkerMsg->getPercentCooldown() );
				break;
			}
		}

	} // process
	//---------------------------------------------------------

	void CWeaponsManager::changeWeapon(unsigned char newWeapon){
		
		//printf("\n%d\t%d",_actualWeapon, newWeapon);
		if(newWeapon >= _numWeapons){
			return;
		}
		if(_weapons[newWeapon] && (newWeapon != _actualWeapon))
		{
			// Indicamos que el arma actual ya no está equipada
			deactivateComponent(_actualWeapon);
			equippedWeapon(_actualWeapon, false);

			activateComponent(newWeapon);
			equippedWeapon(newWeapon, true);

			_actualWeapon = newWeapon;

			Logic::CMessageChangeWeaponGraphics *m=new Logic::CMessageChangeWeaponGraphics();
			m->setWeapon(_actualWeapon);
			_entity->emitMessage(m);
		}
		
	}
	void CWeaponsManager::addAmmo(int ammo, int weapon){
		switch(weapon){
		case 0:
			_entity->getComponent<CShootHammer>("CShootHammer")->addAmmo(0,ammo, (_weapons[weapon]) );
			break;
		case 1:
			_entity->getComponent<CShootSniper>("CShootSniper")->addAmmo(1,ammo, (_weapons[weapon]) );
			break;
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->addAmmo(2,ammo, (_weapons[weapon]) );
			break;
		case 3:
			_entity->getComponent<CShootMiniGun>("CShootMiniGun")->addAmmo(3,ammo, (_weapons[weapon]) );
			break;
		case 4:
			_entity->getComponent<CShootGrenadeLauncher>("CShootGrenadeLauncher")->addAmmo(4,ammo, (_weapons[weapon]) );
			break;
		case 5:
			_entity->getComponent<CShootRocketLauncher>("CShootRocketLauncher")->addAmmo(5,ammo, (_weapons[weapon]) );
			break;
		}
	}

	void CWeaponsManager::resetAmmo(int weapon){
		switch(weapon){
		case 0:
			_entity->getComponent<CShootHammer>("CShootHammer")->resetAmmo();
			break;
		case 1:
			_entity->getComponent<CShootSniper>("CShootSniper")->resetAmmo();
			break;
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->resetAmmo();
			break;
		case 3:
			_entity->getComponent<CShootMiniGun>("CShootMiniGun")->resetAmmo();
			break;
		case 4:
			_entity->getComponent<CShootGrenadeLauncher>("CShootGrenadeLauncher")->resetAmmo();
			break;
		case 5:
			_entity->getComponent<CShootRocketLauncher>("CShootRocketLauncher")->resetAmmo();
			break;
		}
	}

	void CWeaponsManager::amplifyDamage(int percentage) {
		// Amplificamos el daño de todas las armas en base al porcentaje dado
		// Como no tenemos ninguna estructura que almacene las armas, tenemos
		// que listarlas todas a lo bestia.

		_entity->getComponent<CShootHammer>("CShootHammer")->incrementDamage(percentage);
		_entity->getComponent<CShootSniper>("CShootSniper")->incrementDamage(percentage);
		_entity->getComponent<CShootShotGun>("CShootShotGun")->incrementDamage(percentage);
		_entity->getComponent<CShootMiniGun>("CShootMiniGun")->incrementDamage(percentage);
		_entity->getComponent<CShootGrenadeLauncher>("CShootGrenadeLauncher")->incrementDamage(percentage);
		_entity->getComponent<CShootRocketLauncher>("CShootRocketLauncher")->incrementDamage(percentage);
	}

	void CWeaponsManager::reduceCooldowns(int percentage) {
		// Reducimos el cooldown de todas las armas en base al porcentaje dado
		// Como no tenemos ninguna estructura que almacene las armas, tenemos
		// que listarlas todas a lo bestia.

		_entity->getComponent<CShootHammer>("CShootHammer")->reduceCooldown(percentage);
		_entity->getComponent<CShootSniper>("CShootSniper")->reduceCooldown(percentage);
		_entity->getComponent<CShootShotGun>("CShootShotGun")->reduceCooldown(percentage);
		_entity->getComponent<CShootMiniGun>("CShootMiniGun")->reduceCooldown(percentage);
		_entity->getComponent<CShootGrenadeLauncher>("CShootGrenadeLauncher")->reduceCooldown(percentage);
		_entity->getComponent<CShootRocketLauncher>("CShootRocketLauncher")->reduceCooldown(percentage);
	}

	void CWeaponsManager::addWeapon(int ammo, int weapon){
		if(weapon < _numWeapons && !_weapons[weapon])
			_weapons[weapon] = true;

		// Activamos el componente pero indicamos que
		// no es el arma equipada.
		activateComponent(weapon);
		equippedWeapon(weapon, false);

		Logic::CMessageHudAmmo *m=new Logic::CMessageHudAmmo();
		m->setWeapon(weapon);
		m->setAmmo(ammo);//No es necesario esto, ya que solo actualizare el hud como que puedo coger el arma pero no mostrara sus balas(en este caso concreto)
		_entity->emitMessage(m);

		addAmmo(ammo, weapon);
		
	}

	void CWeaponsManager::deactivateComponent(unsigned char weapon){
		CShoot* component;

		switch(weapon){
			case 0:
				component = _entity->getComponent<CShootHammer>("CShootHammer");
				break;
			case 1:
				component = _entity->getComponent<CShootSniper>("CShootSniper");
				break;
			case 2:
				component = _entity->getComponent<CShootShotGun>("CShootShotGun");
				break;
			case 3:
				component = _entity->getComponent<CShootMiniGun>("CShootMiniGun");
				break;
			case 4:
				component = _entity->getComponent<CShootGrenadeLauncher>("CShootGrenadeLauncher");
				break;
			case 5:
				component = _entity->getComponent<CShootRocketLauncher>("CShootRocketLauncher");
				break;
		}

		component->deactivate();
	}

	void CWeaponsManager::activateComponent(unsigned char weapon){
		CShoot* component;

		switch(weapon){
			case 0:
				component = _entity->getComponent<CShootHammer>("CShootHammer");
				break;
			case 1:
				component = _entity->getComponent<CShootSniper>("CShootSniper");
				break;
			case 2:
				component = _entity->getComponent<CShootShotGun>("CShootShotGun");
				break;
			case 3:
				component = _entity->getComponent<CShootMiniGun>("CShootMiniGun");
				break;
			case 4:
				component = _entity->getComponent<CShootGrenadeLauncher>("CShootGrenadeLauncher");
				break;
			case 5:
				component = _entity->getComponent<CShootRocketLauncher>("CShootRocketLauncher");
				break;
		}

		component->activate();	
	}

	void CWeaponsManager::equippedWeapon(unsigned char weapon, bool isEquipped) {
		CShoot* component;

		switch(weapon){
			case 0:
				component = _entity->getComponent<CShootHammer>("CShootHammer");
				break;
			case 1:
				component = _entity->getComponent<CShootSniper>("CShootSniper");
				break;
			case 2:
				component = _entity->getComponent<CShootShotGun>("CShootShotGun");
				break;
			case 3:
				component = _entity->getComponent<CShootMiniGun>("CShootMiniGun");
				break;
			case 4:
				component = _entity->getComponent<CShootGrenadeLauncher>("CShootGrenadeLauncher");
				break;
			case 5:
				component = _entity->getComponent<CShootRocketLauncher>("CShootRocketLauncher");
				break;
		}

		component->inUse(isEquipped);
	}

} // namespace Logic

