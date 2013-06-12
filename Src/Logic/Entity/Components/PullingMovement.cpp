/**
@file FloatingMovement.cpp

@see Logic::IComponent

@author Francisco Aisa García
@author Rubén Mulero Guerrero.
@date Febrero, 2013
*/

#include "PullingMovement.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"
#include "PhysicDynamicEntity.h"
#include "SoulReaper.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"
#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"
#include "Logic/Messages/MessageAddSpell.h"
#include "Logic/Messages/MessageActivate.h"

#include "Net/Manager.h"

namespace Logic {
	
	IMP_FACTORY(CPullingMovement);

//////////////////////////////////////////////////////////////////////////////////////////////////

	CPullingMovement::CPullingMovement(): _player(0), _height(0) {
		// Nada que hacer
	}

//////////////////////////////////////////////////////////////////////////////////////////////////

	void CPullingMovement::onFixedTick(unsigned int msecs) {
		
		//item direction
		Vector3 displacementDir = ( ( _player->getPosition() + Vector3(0,_height,0) ) - _entity->getPosition()).normalisedCopy();
		
		//V = Vo * a * t
		Vector3 displacement = displacementDir * _acceleration * msecs;
		float newVelocity = displacement.length();
		_momentum += newVelocity;
		//displacement*=_momentum/newVelocity;

		//X = Xo * V * t
		displacement*=msecs;

		_physicComponent->move(displacement);
	} // tick

//////////////////////////////////////////////////////////////////////////////////////////////////

	bool CPullingMovement::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(!entityInfo->hasAttribute("acceleration")) {
			return false;
		}
		_acceleration = entityInfo->getFloatAttribute("acceleration");

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
		_physicComponent = _entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity");
		
		return true;
	} // spawn

	bool CPullingMovement::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::TOUCHED;
	} // accept

	void CPullingMovement::setPlayer(CEntity * player){
		_player = player;
		_height = _player->getComponent<CSoulReaper>("CSoulReaper")->getHeight()*0.5;
	}

	
//////////////////////////////////////////////////////////////////////////////////////////////////

	void CPullingMovement::process(const std::shared_ptr<CMessage>& message) {
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

//////////////////////////////////////////////////////////////////////////////////////////////////

	void CPullingMovement::itemGrabbed(CEntity* actor) {
		
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
			else if(_id == "spell") {
				std::shared_ptr<CMessageAddSpell> addSpellMsg = std::make_shared<CMessageAddSpell>();
				addSpellMsg->setSpell(_reward);
				actor->emitMessage(addSpellMsg);
			}
		}
		// borramos esta entidad
		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity, true);

		//resguardo contra bugs, le decimos al soulReaper que no está cogiendo ningun item
		_player->getComponent<CSoulReaper>("CSoulReaper")->resetEntityPulling();
	}

//////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace Logic

