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
#include "Logic/Maps/WorldState.h"

#include "Weapon.h"
#include "ShootShotGun.h"
#include "ShootSniper.h"
#include "ShootMiniGun.h"
#include "ShootHammer.h"
#include "ShootRocketLauncher.h"
#include "IronHellGoat.h"

#include "Logic/Messages/MessageChangeWeapon.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"

#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageHudAmmo.h"

#include "Logic/Messages/MessageDamageAmplifier.h"
#include "Logic/Messages/MessageReducedCooldown.h"


namespace Logic 
{
	IMP_FACTORY(CWeaponsManager);

	//---------------------------------------------------------
	
	CWeaponsManager::CWeaponsManager() : _currentWeapon(0) {
		
	}

	//---------------------------------------------------------

	bool CWeaponsManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		// Inicializamos el vector de armas.
		// De momento no tiene mucho sentido que comprobemos el número de armas que hay
		// por que luego usamos el getComponent a piñon.
		_weaponry.resize(WeaponType::eSIZE);

		// Rellenamos el vector con los punteros a los componentes correspondientes
		_weaponry[WeaponType::eHAMMER].second = _entity->getComponent<CShootHammer>("CShootHammer");
		_weaponry[WeaponType::eSNIPER].second = _entity->getComponent<CShootSniper>("CShootSniper");
		_weaponry[WeaponType::eSHOTGUN].second = _entity->getComponent<CShootShotGun>("CShootShotGun");
		_weaponry[WeaponType::eMINIGUN].second = _entity->getComponent<CShootMiniGun>("CShootMiniGun");
		_weaponry[WeaponType::eIRON_HELL_GOAT].second = _entity->getComponent<CIronHellGoat>("CIronHellGoat");

		/*
		// Por defecto la primera arma está activada y equipada (es el arma 0).
		_weaponry[eHAMMER].first = true;
		_weaponry[eHAMMER].second->stayAvailable();
		_weaponry[eHAMMER].second->inUse(true);

		
		// El resto de las armas están desactivadas, ya que no las tenemos
		for(unsigned int i = 1; i < _weaponry.size(); ++i) {
			_weaponry[i].first = false;
			_weaponry[i].second->stayBusy();
			_weaponry[i].second->inUse(false);
		}
		*/

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CWeaponsManager::onActivate() {
		// El arma actual tiene que ser el hammer, que
		// es la única que tenemos de primeras
		_currentWeapon=WeaponType::eHAMMER;

		// Por defecto la primera arma está activada y equipadda
		_weaponry[WeaponType::eHAMMER].first = true;
		_weaponry[WeaponType::eHAMMER].second->stayAvailable();
//_weaponry[eHAMMER].second->inUse(true);

		// El resto de las armas están desactivadas, ya que no las tenemos
		for(unsigned int i = 1; i < _weaponry.size(); ++i) {
			_weaponry[i].first = false; // Por si acaso habian sido activadas anteriormente
			_weaponry[i].second->stayBusy();
			_weaponry[i].second->resetAmmo();
//_weaponry[i].second->inUse(false);
		}
	} // activate
	
	//---------------------------------------------------------

	void CWeaponsManager::onDeactivate(){
		/*
		//reset de armas y balas
		for(unsigned int i = 0; i<_weaponry.size();++i){
			_weaponry[i].first = false;
			_weaponry[i].second->resetAmmo();
			_weaponry[i].second->inUse(false);
		}
		*/
		
	}// deactivate
	
	//---------------------------------------------------------

	bool CWeaponsManager::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::CHANGE_WEAPON 
			|| msgType == Message::ADD_AMMO
			|| msgType == Message::ADD_WEAPON
			|| msgType == Message::REDUCED_COOLDOWN
			|| msgType == Message::DAMAGE_AMPLIFIER;
	} // accept
	
	//---------------------------------------------------------

	void CWeaponsManager::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CHANGE_WEAPON: {
				std::shared_ptr<CMessageChangeWeapon> changeWeaponMsg = std::static_pointer_cast<CMessageChangeWeapon>(message);
				int iWeapon = changeWeaponMsg->getWeapon();

				//Si iWeapon no es scroll de rueda de ratón hacia adelante o hacia atrás, 
				//asignamos directamente el arma con el índice recibido
				if ((iWeapon != 100) && (iWeapon != -100)){
					changeWeapon(iWeapon);
				}
				else{
					//Obtenemos el índice del arma nueva al que se va a cambiar por el scroll
					iWeapon = selectScrollWeapon(iWeapon);
					if (iWeapon != -1)
						changeWeapon(iWeapon); //Si hemos obtenido arma, se la asignamos
				}
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
			case Message::REDUCED_COOLDOWN: {
				std::shared_ptr<CMessageReducedCooldown> reducedCdMsg = std::static_pointer_cast<CMessageReducedCooldown>(message);
				reduceCooldowns( reducedCdMsg->getPercentCooldown() );
				break;
			}
			case Message::DAMAGE_AMPLIFIER: {
				std::shared_ptr<CMessageDamageAmplifier> damageAmplifierMsg = std::static_pointer_cast<CMessageDamageAmplifier>(message);
				amplifyDamage( damageAmplifierMsg->getPercentDamage() );
				break;
			}
		}

	} // process
	
	//---------------------------------------------------------

	void CWeaponsManager::changeWeapon(unsigned char newWeapon){
		
		//printf("\n%d\t%d",_currentWeapon, newWeapon);
		if(newWeapon >= WeaponType::eSIZE){
			return;
		}
		if(_weaponry[newWeapon].first && (newWeapon != _currentWeapon))
		{
			// Indicamos que el arma actual ya no está equipada
			// Desactivamos el componente Shoot del arma actual
			// e indicamos que ya no está equipada
//_weaponry[_currentWeapon].second->inUse(false);
			_weaponry[_currentWeapon].second->stayBusy();

			// Activamos el componente del nuevo arma que vamos
			// a equipar e indicamos que el arma está equipada
			_weaponry[newWeapon].second->stayAvailable();
//_weaponry[newWeapon].second->inUse(true);
			
			// Actualizamo el indice de arma
			_currentWeapon = newWeapon;

			// Mandamos un mensaje para actualizar el HUD
			std::shared_ptr<CMessageChangeWeaponGraphics> chgWpnGraphicsMsg = std::make_shared<CMessageChangeWeaponGraphics>();
			chgWpnGraphicsMsg->setWeapon(_currentWeapon);
			_entity->emitMessage(chgWpnGraphicsMsg);
			Logic::CWorldState::getSingletonPtr()->addChange(_entity,chgWpnGraphicsMsg);
		}
		
	}

	//---------------------------------------------------------

	void CWeaponsManager::amplifyDamage(int percentage) {
		// Amplificamos el daño de todas las armas en base al porcentaje dado
		for(unsigned int i = 0; i < _weaponry.size(); ++i) {
			_weaponry[i].second->amplifyDamage(percentage);
		}
	}

	//---------------------------------------------------------

	void CWeaponsManager::reduceCooldowns(int percentage) {
		// Reducimos el cooldown de todas las armas en base al porcentaje dado
		for(unsigned int i = 0; i < _weaponry.size(); ++i) {
			_weaponry[i].second->reduceCooldown(percentage);
		}
	}

	//---------------------------------------------------------

	void CWeaponsManager::addWeapon(int ammo, int weaponIndex){
		// Si el arma dada no la teniamos, indicamos que ahora la tenemos
		if(weaponIndex < WeaponType::eSIZE && !_weaponry[weaponIndex].first)
			_weaponry[weaponIndex].first = true;

		// Activamos el componente pero indicamos que
		// no es el arma equipada.
		if(_currentWeapon != weaponIndex){
			_weaponry[weaponIndex].second->stayBusy();
//_weaponry[weaponIndex].second->inUse( false );
		}
		/*
		else{
			//_weaponry[weaponIndex].second->stayAvailable();
			//_weaponry[weaponIndex].second->inUse( true );
		}
		*/
		
		// El arma estara en uso si es la actual, si no estara sin uso
		
		_weaponry[weaponIndex].second->addAmmo(weaponIndex, ammo, _weaponry[weaponIndex].first);

		/*
		// Enviamos un mensaje de actualizacion del hud
		std::shared_ptr<CMessageHudAmmo> *m=std::make_shared<CMessageHudAmmo>();
		m->setWeapon(weaponIndex);
		m->setAmmo(ammo);//No es necesario esto, ya que solo actualizare el hud como que puedo coger el arma pero no mostrara sus balas(en este caso concreto)
		_entity->emitMessage(m);
		*/
	}

	//---------------------------------------------------------

	int CWeaponsManager::selectScrollWeapon(int iWeapon) 
	{
		if (iWeapon == 100) //Armas siguientes
		{
			//Recorremos todas las armas del inventario desde la actual hasta el final
			for (int i = _currentWeapon + 1; i < _weaponry.size(); ++i)
			{
				//Comprobamos si en ese índice tenemos arma
				if (_weaponry[i].first)
					return i;
			}
		}
		else //iWeapon == -100 Armas anteriores
		{
			//Recorremos todas las armas del inventario desde la actual hasta el principio
			for (int i = _currentWeapon - 1; i >= 0; --i)
			{
				//Comprobamos si en ese índice tenemos arma
				if (_weaponry[i].first)
					return i;
			}
		}

		return -1; //No hemos obtenido arma
	}//selectScrollWeapon

} // namespace Logic

