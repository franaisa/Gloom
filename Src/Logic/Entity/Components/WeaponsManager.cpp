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

#include "Logic/Entity/Components/Shoot.h"
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
	
	CWeaponsManager::CWeaponsManager() : _currentWeapon(0), 
										 _numWeapons(6) {
		
	}

	//---------------------------------------------------------

	bool CWeaponsManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		// Inicializamos el vector de armas.
		// De momento no tiene mucho sentido que comprobemos el número de armas que hay
		// por que luego usamos el getComponent a piñon.
		_weaponry.resize(_numWeapons);

		// Rellenamos el vector con los punteros a los componentes correspondientes
		_weaponry[WeaponType::eHAMMER].second = _entity->getComponent<CShootHammer>("CShootHammer");
		_weaponry[WeaponType::eSNIPER].second = _entity->getComponent<CShootSniper>("CShootSniper");
		_weaponry[WeaponType::eSHOTGUN].second = _entity->getComponent<CShootShotGun>("CShootShotGun");
		_weaponry[WeaponType::eMINIGUN].second = _entity->getComponent<CShootMiniGun>("CShootMiniGun");
		_weaponry[WeaponType::eGRENADE_LAUNCHER].second = _entity->getComponent<CShootGrenadeLauncher>("CShootGrenadeLauncher");
		_weaponry[WeaponType::eROCKET_LAUNCHER].second = _entity->getComponent<CShootRocketLauncher>("CShootRocketLauncher");

		// Por defecto la primera arma está activada y equipada (es el arma 0).
		_weaponry[WeaponType::eHAMMER].first = true;
		_weaponry[WeaponType::eHAMMER].second->activate();
		_weaponry[WeaponType::eHAMMER].second->inUse(true);

		// El resto de las armas están desactivadas, ya que no las tenemos
		for(int i = 1; i < _weaponry.size(); ++i) {
			_weaponry[i].first = false;
			_weaponry[i].second->deactivate();
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CWeaponsManager::activate() {
		IComponent::activate();

		// Por defecto la primera arma está activada y equipadda
		_weaponry[WeaponType::eHAMMER].first = true;
		_weaponry[WeaponType::eHAMMER].second->activate();
		_weaponry[WeaponType::eHAMMER].second->inUse(true);

		// El resto de las armas están desactivadas, ya que no las tenemos
		for(int i = 1; i < _weaponry.size(); ++i) {
			_weaponry[i].first = false; // Por si acaso habian sido activadas anteriormente
			_weaponry[i].second->deactivate();
		}
	} // activate
	
	//---------------------------------------------------------

	void CWeaponsManager::deactivate(){
		//reset de armas y balas
		for(int i = 0; i<_weaponry.size();++i){
			_weaponry[i].first = false;
			_weaponry[i].second->resetAmmo();
			_weaponry[i].second->inUse(false);
		}

		// El arma actual tiene que ser el hammer, que
		// es la única que tenemos de primeras
		_currentWeapon=0;
	}// deactivate
	
	//---------------------------------------------------------

	bool CWeaponsManager::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::CHANGE_WEAPON 
			|| msgType == Message::ADD_AMMO
			|| msgType == Message::ADD_WEAPON
			|| msgType == Message::BERSERKER;
	} // accept
	
	//---------------------------------------------------------

	void CWeaponsManager::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CHANGE_WEAPON: {
				std::shared_ptr<CMessageChangeWeapon> changeWeaponMsg = std::static_pointer_cast<CMessageChangeWeapon>(message);
				changeWeapon( changeWeaponMsg->getWeapon() );
				break;
			}
			case Message::ADD_AMMO: {
				std::shared_ptr<CMessageAddAmmo> addAmmoMsg = std::static_pointer_cast<CMessageAddAmmo>(message);
				unsigned int weaponIndex = addAmmoMsg->getAddWeapon();
				_weaponry[weaponIndex].second->addAmmo(weaponIndex, addAmmoMsg->getAddAmmo(), _weaponry[weaponIndex].first);
				break;
			}
			case Message::ADD_WEAPON: {
				std::shared_ptr<CMessageAddWeapon> addWeaponMsg = std::static_pointer_cast<CMessageAddWeapon>(message);
				addWeapon( addWeaponMsg->getAddAmmo(), addWeaponMsg->getAddWeapon() );
				break;
			}
			case Message::BERSERKER: {
				std::shared_ptr<CMessageBerserker> berserkerMsg = std::static_pointer_cast<CMessageBerserker>(message);
				amplifyDamage( berserkerMsg->getPercentDamage() );
				reduceCooldowns( berserkerMsg->getPercentCooldown() );
				break;
			}
		}

	} // process
	
	//---------------------------------------------------------

	void CWeaponsManager::changeWeapon(unsigned char newWeapon){
		
		//printf("\n%d\t%d",_currentWeapon, newWeapon);
		if(newWeapon >= _numWeapons){
			return;
		}
		if(_weaponry[newWeapon].first && (newWeapon != _currentWeapon))
		{
			// Indicamos que el arma actual ya no está equipada
			// Desactivamos el componente Shoot del arma actual
			// e indicamos que ya no está equipada
			_weaponry[_currentWeapon].second->inUse(false);
			_weaponry[_currentWeapon].second->deactivate();

			// Activamos el componente del nuevo arma que vamos
			// a equipar e indicamos que el arma está equipada
			_weaponry[newWeapon].second->activate();
			_weaponry[newWeapon].second->inUse(true);
			
			// Actualizamo el indice de arma
			_currentWeapon = newWeapon;

			// Mandamos un mensaje para actualizar el HUD
			std::shared_ptr<CMessageChangeWeaponGraphics> chgWpnGraphicsMsg = std::make_shared<CMessageChangeWeaponGraphics>();
			chgWpnGraphicsMsg->setWeapon(_currentWeapon);
			_entity->emitMessage(chgWpnGraphicsMsg);
		}
		
	}

	//---------------------------------------------------------

	void CWeaponsManager::amplifyDamage(int percentage) {
		// Amplificamos el daño de todas las armas en base al porcentaje dado
		for(int i = 0; i < _weaponry.size(); ++i) {
			_weaponry[i].second->incrementDamage(percentage);
		}
	}

	//---------------------------------------------------------

	void CWeaponsManager::reduceCooldowns(int percentage) {
		// Reducimos el cooldown de todas las armas en base al porcentaje dado
		for(int i = 0; i < _weaponry.size(); ++i) {
			_weaponry[i].second->reduceCooldown(percentage);
		}
	}

	//---------------------------------------------------------

	void CWeaponsManager::addWeapon(int ammo, int weaponIndex){
		// Si el arma dada no la teniamos, indicamos que ahora la tenemos
		if(weaponIndex < _numWeapons && !_weaponry[weaponIndex].first)
			_weaponry[weaponIndex].first = true;

		// Activamos el componente pero indicamos que
		// no es el arma equipada.
		_weaponry[weaponIndex].second->activate();
		
		// El arma estara en uso si es la actual, si no estara sin uso
		_weaponry[weaponIndex].second->inUse( _currentWeapon == weaponIndex );
		_weaponry[weaponIndex].second->addAmmo(weaponIndex, ammo, _weaponry[weaponIndex].first);

		/*
		// Enviamos un mensaje de actualizacion del hud
		std::shared_ptr<CMessageHudAmmo> *m=std::make_shared<CMessageHudAmmo>();
		m->setWeapon(weaponIndex);
		m->setAmmo(ammo);//No es necesario esto, ya que solo actualizare el hud como que puedo coger el arma pero no mostrara sus balas(en este caso concreto)
		_entity->emitMessage(m);
		*/
	}

} // namespace Logic

