/**
@file ExplotionController.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "GrenadeController.h"

#include "Physics/Server.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/GameNetMsgManager.h"

#include "Logic/Entity/Components/Graphics.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddForcePlayer.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

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

		_explotionDamage = entityInfo->getFloatAttribute("explotionDamage");
		_explotionRadius = entityInfo->getFloatAttribute("explotionRadius");

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
		// Hacemos una consulta de overlap para ver si tenemos que mandar
		// un mensaje de daño
		CEntity** entitiesHit = NULL;
		int nbHits = 0;
		// Hacemos una query de overlap en la posicion en la que se encuentra la granada con el radio
		// que se indique de explosion
		Physics::CServer::getSingletonPtr()->overlapExplotion(_entity->getPosition(), _explotionRadius, entitiesHit, nbHits);

		// Mandamos el mensaje de daño a cada una de las entidades que hayamos golpeado
		// Además aplicamos un desplazamiento al jugador 
		for(int i = 0; i < nbHits; ++i) {
			if(entitiesHit[i] != NULL) {
				CMessageDamaged* msg = new CMessageDamaged();
				msg->setDamage( _explotionDamage );
				msg->setEnemy(_owner);
				entitiesHit[i]->emitMessage(msg);
				//Mensaje desplazamiento
				CMessageAddForcePlayer* msg2 = new CMessageAddForcePlayer();
				msg2->setPower(0.1f);
				msg2->setVelocity(0.12f);
				Vector3 direccionImpacto=(entitiesHit[i]->getPosition()-_entity->getPosition());
				direccionImpacto.normalise();
				msg2->setDirection(direccionImpacto);
				entitiesHit[i]->emitMessage(msg2);
			}
		}

		if(nbHits > 0) delete [] entitiesHit;
		/*
		// Obtenemos la informacion asociada al arquetipo de la explosion de la granada
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Explotion");
		// Creamos la entidad y la activamos
		CEntity* grenadeExplotion = CEntityFactory::getSingletonPtr()->createEntityWithTimeOut(entityInfo, Logic::CServer::getSingletonPtr()->getMap(), 200);
		grenadeExplotion->activate();

		// Debido a que la explosion es un mero grafico, situamos la entidad grafica
		// en el centro de la posicion en la que esta la granada (restando el radio ya
		// que los graficos tienen el pivote en el pie).
		CGraphics* graphicComponent = grenadeExplotion->getComponent<CGraphics>("CGraphics");
		assert(graphicComponent != NULL && "No se puede colocar la explosion porque no tiene componente grafico");
		graphicComponent->setTransform( _entity->getTransform() );
		*/

		Graphics::CParticle *particle = Graphics::CServer::getSingletonPtr()->getActiveScene()->createParticle(_entity->getName(),"ExplosionParticle", _entity->getPosition());
	} // createExplotion

	//________________________________________________________________________

	void CGrenadeController::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

} // namespace Logic

