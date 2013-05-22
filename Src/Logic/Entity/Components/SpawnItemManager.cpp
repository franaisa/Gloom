/**
@file SpawnItemManager.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "SpawnItemManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/WorldState.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/Graphics.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"
#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"
#include "Logic/Messages/MessageActivate.h"


#include "Net/Manager.h"


namespace Logic {
	
	IMP_FACTORY(CSpawnItemManager);
	
	//________________________________________________________________________

	void CSpawnItemManager::onTick(unsigned int msecs) {
		// Si el item esta en la fase de respawn, comprobamos si se ha cumplido el tiempo
		// limite para resucitar las entidades graficas y fisicas.
		if(_isRespawning) {
			_timer += msecs;

			if(_timer >= _respawnTime) {
				_isRespawning = false;
				_timer = 0;
				// Activar entidad grafica y fisica
				std::shared_ptr<CMessageActivate> activateMsg = std::make_shared<CMessageActivate>();
				activateMsg->setActivated(true);
				Logic::CWorldState::getSingletonPtr()->addChange(_entity,activateMsg);
				_entity->emitMessage(activateMsg);
			}
		}
	} // tick

	//________________________________________________________________________

	bool CSpawnItemManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// El id indica el tipo del item: orbe, arma, municion...
		if(entityInfo->hasAttribute("id")) {
			_id = entityInfo->getStringAttribute("id");

			// Si el item es un arma o municion, comprobamos de que arma en concreto se trata
			// El atributo weaponType solo es necesario en caso de que el item sea un arma
			// o municion
			if(_id == "weapon" || _id == "ammo") {
				if(entityInfo->hasAttribute("weaponType")) {
					_weaponType = entityInfo->getIntAttribute("weaponType");
				}
			}
		}

		// Puntos de beneficio del item
		if(entityInfo->hasAttribute("reward")) {
			_reward = entityInfo->getIntAttribute("reward");
		}

		// Tiempo de respawn del item en segundos
		if(entityInfo->hasAttribute("respawnTime")) {
			_respawnTime = entityInfo->getFloatAttribute("respawnTime");
			_respawnTime *= 1000; // Convertimos en milisegundos
		}

		return true;

	} // spawn
	
	//________________________________________________________________________

	bool CSpawnItemManager::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//________________________________________________________________________

	void CSpawnItemManager::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				std::shared_ptr<CMessageTouched> touchedMsg = std::static_pointer_cast<CMessageTouched>(message);
				// Si se ha disparado el trigger del item recompensamos a la entidad
				// que ha disparado el trigger con la ventaja que de el item cogido.
				itemGrabbed( touchedMsg->getEntity() );
				break;
			}
		}
	} // process

	//________________________________________________________________________

	void CSpawnItemManager::itemGrabbed(CEntity* actor) {

		// Desactivamos la entidad grafica y fisica.
		std::shared_ptr<CMessageActivate> deactivateMsg = std::make_shared<CMessageActivate>();
		deactivateMsg->setActivated(false);
		_entity->emitMessage(deactivateMsg);
		Logic::CWorldState::getSingletonPtr()->addChange(_entity,deactivateMsg);
		// Si se trata del servidor o del single player
		if(Net::CManager::getSingletonPtr()->imServer() || (!Net::CManager::getSingletonPtr()->imServer() && !Net::CManager::getSingletonPtr()->imClient())){
			// Mandar el mensaje que corresponda a la entidad actuadora
			// en funcion del item que se haya cogido (comprobando el id)
			if(_id == "orb") {
				std::shared_ptr<CMessageAddLife> addLifeMsg = std::make_shared<CMessageAddLife>();
				addLifeMsg->setAddLife(_reward);
				actor->emitMessage(addLifeMsg);
			}
			else if(_id == "armor") {
				std::shared_ptr<CMessageAddShield> addShieldMsg = std::make_shared<CMessageAddShield>();
				addShieldMsg->setAddShield(_reward);
				actor->emitMessage(addShieldMsg);
			}
			else if(_id == "ammo") {
				std::shared_ptr<CMessageAddAmmo> addAmmoMsg = std::make_shared<CMessageAddAmmo>();
				addAmmoMsg->setAddAmmo(_reward);
				addAmmoMsg->setAddWeapon(_weaponType);
				actor->emitMessage(addAmmoMsg);
			}
			else if(_id == "weapon") {
				std::shared_ptr<CMessageAddWeapon> addWeaponMsg = std::make_shared<CMessageAddWeapon>();
				addWeaponMsg->setAddAmmo(_reward);
				addWeaponMsg->setAddWeapon(_weaponType);
				actor->emitMessage(addWeaponMsg);
			}
		}
		// Arrancamos el timer.
		_isRespawning = true;
	}

} // namespace Logic

