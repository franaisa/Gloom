/**
@file RocketController.cpp

@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#include "RocketController.h"

#include "Physics/Server.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/GameNetMsgManager.h"

#include "Logic/Entity/Components/Graphics.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageKinematicMove.h"
#include "Logic/Messages/MessageDamaged.h"

namespace Logic {
	
	IMP_FACTORY(CRocketController);
	
	//________________________________________________________________________

	void CRocketController::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		// Movemos el cohete
		// Mensaje para situar el collider fisico de la granada en la posicion de disparo.
		Logic::CMessageKinematicMove* msg = new Logic::CMessageKinematicMove();
		Vector3 direction= _direction*msecs*_rocketSpeed;
		msg->setMovement(direction);
		_entity->emitMessage(msg);
	
	} // tick

	//________________________________________________________________________

	bool CRocketController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;
			
		_rocketSpeed = entityInfo->getFloatAttribute("rocketSpeed");
		_explotionDamage = entityInfo->getFloatAttribute("explotionDamage");
		_explotionRadius = entityInfo->getFloatAttribute("explotionRadius");

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
		// Hacemos una consulta de overlap para ver si tenemos que mandar
		// un mensaje de daño
		CEntity** entitiesHit = NULL;
		int nbHits = 0;
		// Hacemos una query de overlap en la posicion en la que se encuentra la granada con el radio
		// que se indique de explosion
		Physics::CServer::getSingletonPtr()->overlapExplotion(_entity->getPosition(), _explotionRadius, entitiesHit, nbHits);

		// Mandamos el mensaje de daño a cada una de las entidades que hayamos golpeado
		for(int i = 0; i < nbHits; ++i) {
			if(entitiesHit[i] != NULL) {
				CMessageDamaged* msg = new CMessageDamaged();
				msg->setDamage( _explotionDamage );
				msg->setEnemy(_owner);
				entitiesHit[i]->emitMessage(msg);
			}
		}

		if(nbHits > 0) delete [] entitiesHit;

		// Obtenemos la informacion asociada al arquetipo de la explosion del cohete
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Explotion");
		// Creamos la entidad y la activamos
		CEntity* rocketExplotion = CEntityFactory::getSingletonPtr()->createEntityWithTimeOut(entityInfo, Logic::CServer::getSingletonPtr()->getMap(), 200);
		rocketExplotion->activate();

		// Debido a que la explosion es un mero grafico, situamos la entidad grafica
		// en el centro de la posicion en la que esta el misil (restando el radio ya
		// que los graficos tienen el pivote en el pie).
		CGraphics* graphicComponent = rocketExplotion->getComponent<CGraphics>("CGraphics");
		assert(graphicComponent != NULL && "No se puede colocar la explosion porque no tiene componente grafico");
		graphicComponent->setTransform( _entity->getTransform() );
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

