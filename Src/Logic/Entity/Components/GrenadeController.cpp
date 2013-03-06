/**
@file ExplotionController.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "GrenadeController.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/GameNetMsgManager.h"

#include "Logic/Entity/Components/ExplotionHitNotifier.h"
#include "Logic/Entity/Components/PhysicEntity.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageContactEnter.h"

namespace Logic {
	
	IMP_FACTORY(CGrenadeController);
	
	//________________________________________________________________________

	void CGrenadeController::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		// Actualizamos el timer. Si se ha cumplido el tiempo limite de explosion
		// eliminamos la entidad granada y creamos la entidad explosion.
		_timer += msecs;
		if(_timer > _explotionTime || _enemyHit) {
			// Eliminamos la entidad en diferido
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			Logic::CGameNetMsgManager::getSingletonPtr()->sendDestroyEntity(_entity->getEntityID());
			// Creamos la explosion
			createExplotion();
		}
	} // tick

	//________________________________________________________________________

	bool CGrenadeController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// Leer el timer que controla la explosion
		if( entityInfo->hasAttribute("explotionTime") ) {
			// Pasamos a msecs
			_explotionTime = entityInfo->getFloatAttribute("explotionTime") * 1000;
		}

		return true;
	} // spawn

	//________________________________________________________________________

	bool CGrenadeController::accept(CMessage *message) {
		return (message->getMessageType() == Message::CONTACT_ENTER);
	} // accept
	
	//________________________________________________________________________

	void CGrenadeController::process(CMessage *message) {
		switch(message->getMessageType()) {
		case Message::CONTACT_ENTER:
			// Las granadas solo notifican de contacto contra players y por lo
			// tanto al recibir este mensaje signfica que ha impactado contra
			// otro player
			_enemyHit = true;

			break;
		}
	} // process

	//________________________________________________________________________

	void CGrenadeController::createExplotion() {
		// Obtenemos la informacion asociada al arquetipo de la explosion de la granada
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Explotion");
		// Creamos la entidad y la activamos
		CEntity* grenadeExplotion = CEntityFactory::getSingletonPtr()->createEntityWithTimeOut(entityInfo, Logic::CServer::getSingletonPtr()->getMap(), 200);
		grenadeExplotion->activate();

		// Enviamos el mensaje para situar a la explosion en el punto en el que estaba la granada
		Logic::CMessageSetPhysicPosition* msg = new Logic::CMessageSetPhysicPosition();
		msg->setPosition( _entity->getPosition() ); // spawneamos la explosion justo en el centro de la granada
		msg->setMakeConversion(false);
		grenadeExplotion->emitMessage(msg);

		// Seteamos la entidad que dispara la granada
		CExplotionHitNotifier* comp = grenadeExplotion->getComponent<CExplotionHitNotifier>("CExplotionHitNotifier");
		assert(comp != NULL);
		comp->setOwner(_owner);
	} // createExplotion

	//________________________________________________________________________

	void CGrenadeController::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

} // namespace Logic

