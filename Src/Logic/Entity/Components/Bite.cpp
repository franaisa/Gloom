#include "Bite.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageDamaged.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

namespace Logic{

	IMP_FACTORY(CBite);

	bool CBite::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("damage") && "Error: No se ha definido el atributo damage en el arquetipos" );
		_biteDamage = entityInfo->getFloatAttribute("damage");

		return true;
	} // spawn


	bool CBite::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::TOUCHED;
	}

	void CBite::process(const std::shared_ptr<CMessage>& message){
		if ( message->getMessageType() == Message::TOUCHED){
			std::shared_ptr<CMessageTouched> msg = std::static_pointer_cast<CMessageTouched>(message);
			CEntity* entity = msg->getEntity();

			std::shared_ptr<CMessageDamaged> damage = std::make_shared<CMessageDamaged>();
			damage->setEnemy(_owner);
			damage->setDamage(_biteDamage);
			entity->emitMessage(damage);
		}
	}

	void CBite::setOwner(CEntity *owner){
		_owner = owner;
	}
}