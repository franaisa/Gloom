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
#include "Logic/Entity/Components/PhysicEntity.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"
#include "Net/Manager.h"

namespace Logic 
{
	IMP_FACTORY(CSpawnItemManager);
	
	//---------------------------------------------------------
	void CSpawnItemManager::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		if(_isRespawning) {
			_timer += msecs;

			if(_timer >= _respawnTime) {
				_isRespawning = false;
				_timer = 0;

				// Activar entidad grafica
				_entity->getComponent<CGraphics>("CGraphics")->activate();
				_entity->getComponent<CGraphics>("CGraphics")->setVisible(true);

				// Activar entidad fisica (solo si soy el servidor o single player)
				if(Net::CManager::getSingletonPtr()->imServer() || (!Net::CManager::getSingletonPtr()->imServer() && Net::CManager::getSingletonPtr()->imClient()))
					_entity->getComponent<CPhysicEntity>("CPhysicEntity")->activate();
			}
		}
	} // tick

	//---------------------------------------------------------
	bool CSpawnItemManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("id")) {
			_id = entityInfo->getStringAttribute("id");

			if(_id == "weapon" || _id == "ammo") {
				if(entityInfo->hasAttribute("weaponType")) {
					_weaponType = entityInfo->getIntAttribute("weaponType");
				}
			}
		}
		if(entityInfo->hasAttribute("reward")) {
			_reward = entityInfo->getIntAttribute("reward");
		}
		if(entityInfo->hasAttribute("respawnTime")) {
			_respawnTime = entityInfo->getFloatAttribute("respawnTime");
			_respawnTime *= 1000; // Convertimos en segundos
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------
	bool CSpawnItemManager::accept(CMessage *message) {
		return (message->getMessageType() == Message::TOUCHED);
	} // accept
	
	//---------------------------------------------------------
	void CSpawnItemManager::process(CMessage *message) {
		switch(message->getMessageType()) {
		case Message::TOUCHED:
			itemGrabbed( ((CMessageTouched*)message)->getEntity() );
			break;
		}
	} // process

	//---------------------------------------------------------
	void CSpawnItemManager::itemGrabbed(CEntity* actor) {

		// Desactivar entidad grafica
		_entity->getComponent<CGraphics>("CGraphics")->deactivate();
		_entity->getComponent<CGraphics>("CGraphics")->setVisible(false);
		std::cout << "me llega mensaje de itemgrabbed" << std::endl;
		
		if(Net::CManager::getSingletonPtr()->imServer() || (!Net::CManager::getSingletonPtr()->imServer() && !Net::CManager::getSingletonPtr()->imClient())){
			// Activar entidad fisica
			_entity->getComponent<CPhysicEntity>("CPhysicEntity")->deactivate();
		
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
				// Mandar un mensaje con el _weaponType
				//CMessageAddAmmo* m = new CMessageAddAmmo();
				//m->setQuantity(_reward);
				//m->setWeaponType(_weaponType);
				//actor->emitMessage(m);
			}
			else if(_id == "weapon") {
				// Mandar un mensaje con el _weaponType
				//CMessageAddWeapon* m = new CMessageAddWeapon();
				//m->setQuantity(_reward);
				//m->setWeaponType(_weaponType);
				//actor->emitMessage(m);
			}
		}
		// Arrancar el timer
		_isRespawning = true;
	}

} // namespace Logic

