/**
@file ExplotionController.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "ExplotionHitNotifier.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageDamaged.h"

namespace Logic {
	
	IMP_FACTORY(CExplotionHitNotifier);
	
	bool CExplotionHitNotifier::accept(CMessage *message) {
		return (message->getMessageType() == Message::TOUCHED);
	} // accept
	
	//________________________________________________________________________

	void CExplotionHitNotifier::process(CMessage *message) {
		switch(message->getMessageType()) {
		case Message::TOUCHED:
			// Si se ha disparado el trigger de la explosion, calculamos
			// cuanto daño se ha hecho el actor que lo ha disparado
			explotionHit( ((CMessageTouched*)message)->getEntity() );
			break;
		}
	} // process

	//________________________________________________________________________

	bool CExplotionHitNotifier::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if( entityInfo->hasAttribute("explotionDamage") ) {
			// Daño máximo de la explosion
			_explotionDamage = entityInfo->getFloatAttribute("explotionDamage");
		}
		if( entityInfo->hasAttribute("physic_radius") ) {
			_explotionRadius = entityInfo->getFloatAttribute("physic_radius");
		}

		return true;

	} // spawn

	//________________________________________________________________________

	void CExplotionHitNotifier::explotionHit(CEntity* entity) {
		if(entity == NULL) return;
		float entityDistanceFromZero = this->_entity->getPosition().distance( entity->getPosition() );

		// Quitamos vida al jugador en proporcion a la distancia que se encuentra
		// de la zona cero
		CMessageDamaged* msg = new CMessageDamaged();
		msg->setDamage( _explotionDamage * (1 - (entityDistanceFromZero / _explotionRadius)) );
		msg->setEnemy(_owner);
		entity->emitMessage(msg);

		float res = _explotionDamage * (1 - (entityDistanceFromZero / _explotionRadius));

		std::cout << "Te quito " << res << " puntos de salud" << std::endl;
	}

	//________________________________________________________________________

	void CExplotionHitNotifier::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

} // namespace Logic

