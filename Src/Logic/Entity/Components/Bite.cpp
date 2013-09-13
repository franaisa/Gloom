#include "Bite.h"
#include "PhysicDynamicEntity.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddLife.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

namespace Logic{

	IMP_FACTORY(CBite);

	bool CBite::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("damage") && "Error: No se ha definido el atributo damage en el arquetipos" );
		assert( entityInfo->hasAttribute("lifeSteal") );

		_biteDamage = entityInfo->getFloatAttribute("damage");
		_lifeSteal = entityInfo->getFloatAttribute("lifeSteal");

		return true;
	} // spawn


	bool CBite::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::TOUCHED;
	}

	void CBite::process(const std::shared_ptr<CMessage>& message){
		if ( message->getMessageType() == Message::TOUCHED){
			std::shared_ptr<CMessageTouched> msg = std::static_pointer_cast<CMessageTouched>(message);
			CEntity* entityTouched = msg->getEntity();

			if(_owner != NULL && entityTouched != _owner) {
				std::shared_ptr<CMessageDamaged> damage = std::make_shared<CMessageDamaged>();
				damage->setEnemy(_owner);
				damage->setDamage(_biteDamage);
				entityTouched->emitMessage(damage);

				std::shared_ptr<CMessageAddLife> addLifeMsg = std::make_shared<CMessageAddLife>();
				addLifeMsg->setAddLife(_lifeSteal);
				_owner->emitMessage(addLifeMsg);
			}
		}
	}

	void CBite::setOwner(CEntity *owner){
		_owner = owner;
	}

	void CBite::onStart() {
		// Sacamos el componente fisico
		_physicComponent = _entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity");
		assert(_physicComponent != NULL);
	}

	void CBite::onTick(unsigned int msecs) {
		if(_owner != NULL)
			_physicComponent->setPosition(_owner->getPosition(), true);
	}
}