/**
@file ExplotionController.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "ExplotionHitNotifier.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/PhysicEntity.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageDamaged.h"

namespace Logic {
	
	IMP_FACTORY(CExplotionHitNotifier);

	//________________________________________________________________________

	void CExplotionHitNotifier::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		// Actualizamos el timer. Si se ha cumplido el tiempo limite de explosion
		// eliminamos la entidad granada y creamos la entidad explosion.
		_timer += msecs;
		if(_timer > _explotionTimeOut) {
			// Desactivamos la entidad fisica (para que no se dispare el trigger
			// al eliminar la explosion, ya que se detecta que el shape ya no esta)
			// Seteamos la entidad que dispara la granada
			CPhysicEntity* physicEntityComponent = _entity->getComponent<CPhysicEntity>("CPhysicEntity");
			assert(physicEntityComponent != NULL);
			physicEntityComponent->deactivate();

			// Eliminamos la entidad en diferido
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
	} // tick

	//________________________________________________________________________
	
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

		// Leer el timer que controla la explosion
		if( entityInfo->hasAttribute("explotionTime") ) {
			// Pasamos a msecs
			_explotionTimeOut = entityInfo->getFloatAttribute("explotionTime") * 1000;
		}
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

		std::cout << "Soy "<< _owner->getName() << " y te quito " << res << " puntos de salud" << std::endl;
	}

	//________________________________________________________________________

	void CExplotionHitNotifier::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

} // namespace Logic

