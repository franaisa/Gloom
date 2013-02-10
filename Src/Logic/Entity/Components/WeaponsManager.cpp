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
#include "Logic/Entity/Components/ShootMiniGun.h"
#include "Logic/Entity/Components/ShootHammer.h"


#include "Logic/Messages/MessageChangeWeapon.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"

#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageHudAmmo.h"




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
			for(int i = 0; i<_numWeapons;++i){
				_weapons[i] = false;
				desactivateComponent(i);
			}

			/*la 1º arama siempre estara a true*/
			_weapons[0] = true;
			activateComponent(_actualWeapon);
			/*
			_weapons[1] = true;
			_weapons[2] = true;
			printf("\n\n mi actual weapon es.....%d", _actualWeapon);

			*/
		}
		return true;

	} // spawn
	//---------------------------------------------------------

	void CWeaponsManager::activate()
	{
		IComponent::activate();

		for(int i = 0; i<_numWeapons;++i){
				desactivateComponent(i);
			}

			/*la 1º arma siempre estara a true*/
			activateComponent(_actualWeapon);

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
	
	}// deactivate
	//---------------------------------------------------------


	bool CWeaponsManager::accept(CMessage *message)
	{
		return message->getMessageType() == Message::CHANGE_WEAPON 
			|| message->getMessageType() == Message::ADD_AMMO
			|| message->getMessageType() == Message::ADD_WEAPON;
	} // accept
	//---------------------------------------------------------


	void CWeaponsManager::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
			case Message::CHANGE_WEAPON:
				changeWeapon( ((CMessageChangeWeapon*)message)->getWeapon() );
			break;
			case Message::ADD_AMMO:
				addAmmo( ((CMessageAddAmmo*)message)->getAddAmmo(),((CMessageAddAmmo*)message)->getAddWeapon()  );
			break;
			case Message::ADD_WEAPON:
				addWeapon( ((CMessageAddWeapon*)message)->getAddAmmo(),((CMessageAddWeapon*)message)->getAddWeapon()  );
			break;
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

			desactivateComponent(_actualWeapon);
			_actualWeapon = newWeapon;
			activateComponent(newWeapon);

			Logic::CMessageChangeWeaponGraphics *m=new Logic::CMessageChangeWeaponGraphics();
			m->setWeapon(_actualWeapon);
			_entity->emitMessage(m);
		}
		
	}
	void CWeaponsManager::addAmmo(int ammo, int weapon){
		switch(weapon){
		case 0:
			_entity->getComponent<CShootHammer>("CShootHammer")->addAmmo(0,ammo, (weapon == _weapons[weapon]) );
			break;
		case 1:
			_entity->getComponent<CShootMiniGun>("CShootMiniGun")->addAmmo(1,ammo, (weapon == _weapons[weapon]) );
			break;
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->addAmmo(2,ammo, (weapon == _weapons[weapon]) );
			break;
		}
	}

	void CWeaponsManager::resetAmmo(int weapon){
		switch(weapon){
		case 0:
			_entity->getComponent<CShootHammer>("CShootHammer")->resetAmmo();
			break;
		case 1:
			_entity->getComponent<CShootMiniGun>("CShootMiniGun")->resetAmmo();
			break;
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->resetAmmo();
			break;
		}
	}


	void CWeaponsManager::addWeapon(int ammo, int weapon){
		if(weapon < _numWeapons && !_weapons[weapon])
			_weapons[weapon] = true;

		Logic::CMessageHudAmmo *m=new Logic::CMessageHudAmmo();
		m->setWeapon(weapon);
		m->setAmmo(ammo);//No es necesario esto, ya que solo actualizare el hud como que puedo coger el arma pero no mostrara sus balas(en este caso concreto)
		_entity->emitMessage(m);

		addAmmo(ammo, weapon);
		
	}

	void CWeaponsManager::desactivateComponent(unsigned char weapon){
		switch(weapon){
		case 0:
			_entity->getComponent<CShootHammer>("CShootHammer")->deactivate();
			break;
		case 1:
			_entity->getComponent<CShootMiniGun>("CShootMiniGun")->deactivate();
			break;
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->deactivate();
			break;

		/*
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->deactivate();
			break;
		*/
		}
	}

	void CWeaponsManager::activateComponent(unsigned char weapon){
		switch(weapon){
		case 0:
			_entity->getComponent<CShootHammer>("CShootHammer")->activate();
			break;
		case 1:
			_entity->getComponent<CShootMiniGun>("CShootMiniGun")->activate();
			break;
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->activate();
			break;

		/*
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->deactivate();
			break;
		*/
		}
	}

} // namespace Logic

