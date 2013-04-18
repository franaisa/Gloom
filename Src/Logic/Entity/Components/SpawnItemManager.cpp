/**
@file SpawnItemManager.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "SpawnItemManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/Graphics.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"
#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"
#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageDeactivate.h"

#include "Net/Manager.h"


namespace Logic {
	
	IMP_FACTORY(CSpawnItemManager);
	
	//________________________________________________________________________

	void CSpawnItemManager::tick(unsigned int msecs) {
		// Si el item esta en la fase de respawn, comprobamos si se ha cumplido el tiempo
		// limite para resucitar las entidades graficas y fisicas.
		if(_isRespawning) {
			_timer += msecs;

			if(_timer >= _respawnTime) {
				_isRespawning = false;
				_timer = 0;

				// Activar entidad grafica y fisica
				CMessageActivate* m = new CMessageActivate();
				_entity->emitMessage(m);

				// Activar la entidad fisica (solo si soy el servidor o single player)
				if(Net::CManager::getSingletonPtr()->imServer() || (!Net::CManager::getSingletonPtr()->imServer() && !Net::CManager::getSingletonPtr()->imClient()))
					;//_entity->getComponent<CPhysicEntity>("CPhysicEntity")->activate();
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
			_respawnTime *= 1000; // Convertimos en segundos
		}

		return true;

	} // spawn
	
	//________________________________________________________________________

	bool CSpawnItemManager::accept(CMessage *message) {
		return (message->getMessageType() == Message::TOUCHED);
	} // accept
	
	//________________________________________________________________________

	void CSpawnItemManager::process(CMessage *message) {
		switch(message->getMessageType()) {
		case Message::TOUCHED:
			// Si se ha disparado el trigger del item recompensamos a la entidad
			// que ha disparado el trigger con la ventaja que de el item cogido.
			itemGrabbed( ((CMessageTouched*)message)->getEntity() );
			break;
		}
	} // process

	//________________________________________________________________________

	void CSpawnItemManager::itemGrabbed(CEntity* actor) {

		// Desactivamos la entidad grafica y fisica.
		CMessageDeactivate* m = new CMessageDeactivate();
		_entity->emitMessage(m);
		
		// Si se trata del servidor o del single player
		if(Net::CManager::getSingletonPtr()->imServer() || (!Net::CManager::getSingletonPtr()->imServer() && !Net::CManager::getSingletonPtr()->imClient())){
			// Desactivar entidad fisica
			//_entity->getComponent<CPhysicEntity>("CPhysicEntity")->deactivate();
		
			// Mandar el mensaje que corresponda a la entidad actuadora
			// en funcion del item que se haya cogido (comprobando el id)
			if(_id == "orb") {
				CMessageAddLife* m = new CMessageAddLife();
				m->setAddLife(_reward);
				actor->emitMessage(m);
			}
			else if(_id == "armor") {
				CMessageAddShield* m = new CMessageAddShield();
				m->setAddShield(_reward);
				actor->emitMessage(m);
			}
			else if(_id == "ammo") {
				CMessageAddAmmo* m = new CMessageAddAmmo();
				m->setAddAmmo(_reward);
				m->setAddWeapon(_weaponType);
				actor->emitMessage(m);
			}
			else if(_id == "weapon") {
				CMessageAddWeapon* m = new CMessageAddWeapon();
				m->setAddAmmo(_reward);
				m->setAddWeapon(_weaponType);
				actor->emitMessage(m);
			}
		}

		// Arrancamos el timer.
		_isRespawning = true;
	}

} // namespace Logic

