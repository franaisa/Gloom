/**
@file RocketController.cpp

@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#include "RocketController.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/GameNetMsgManager.h"

#include "Logic/Entity/Components/ExplotionHitNotifier.h"
#include "Logic/Entity/Components/PhysicEntity.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageKinematicMove.h"

namespace Logic {
	
	IMP_FACTORY(CRocketController);
	
	//________________________________________________________________________

	void CRocketController::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		// Movemos el cohete
		// Mensaje para situar el collider fisico de la granada en la posicion de disparo.
		Logic::CMessageKinematicMove* msg = new Logic::CMessageKinematicMove();
		Vector3 direction= _direction*msecs*0.10;//velocidad a poner parametrizable
		msg->setMovement(direction);
		_entity->emitMessage(msg);

	} // tick

	//________________________________________________________________________

	bool CRocketController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;
	} // spawn

	//________________________________________________________________________

	bool CRocketController::accept(CMessage *message) {
		return (message->getMessageType() == Message::CONTACT_ENTER);
	} // accept
	
	//________________________________________________________________________

	void CRocketController::process(CMessage *message) {
		switch(message->getMessageType()) {
		case Message::CONTACT_ENTER:
			// Los cohetes solos se destruyen al colisionar
			// Este mensaje deberia recibirse al tocar cualquier otro
			// rigidbody del mundo

			// Eliminamos la entidad en diferido
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			Logic::CGameNetMsgManager::getSingletonPtr()->sendDestroyEntity(_entity->getEntityID());
			// Creamos la explosion
			createExplotion();

			break;
		}
	} // process

	//________________________________________________________________________

	void CRocketController::createExplotion() {
		// Obtenemos la informacion asociada al arquetipo de la explosion del cohete
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Explotion");
		// Creamos la entidad y la activamos
		CEntity* rocketExplotion = CEntityFactory::getSingletonPtr()->createEntity( entityInfo, Logic::CServer::getSingletonPtr()->getMap(), 200 );
		rocketExplotion->activate();

		// Enviamos el mensaje para situar a la explosion en el punto en el que estaba el cohete
		Logic::CMessageSetPhysicPosition* msg = new Logic::CMessageSetPhysicPosition();
		msg->setPosition( _entity->getPosition() ); // spawneamos la explosion justo en el centro del cohete
		msg->setMakeConversion(false);
		rocketExplotion->emitMessage(msg);

		// Seteamos la entidad que dispara el cohete
		CExplotionHitNotifier* comp = rocketExplotion->getComponent<CExplotionHitNotifier>("CExplotionHitNotifier");
		assert(comp != NULL);
		comp->setOwner(_owner);
	} // createExplotion

	//________________________________________________________________________

	void CRocketController::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

	//________________________________________________________________________

	void CRocketController::setDirection(Vector3 direction) {
		_direction = direction;
	}

} // namespace Logic

