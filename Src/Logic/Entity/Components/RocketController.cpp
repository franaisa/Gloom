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

#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageKinematicMove.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddForcePlayer.h"
#include "Logic/Messages/MessageCreateParticle.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

namespace Logic {
	
	IMP_FACTORY(CRocketController);

	CRocketController::CRocketController() : _enemyHit(false) { 
		// Nada que hacer
	}

	//________________________________________________________________________

	bool CRocketController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;
			
		_explotionDamage = entityInfo->getFloatAttribute("explotionDamage");
		_explotionRadius = entityInfo->getFloatAttribute("explotionRadius");
		_audioExplotion = entityInfo->getStringAttribute("explotionAudio");
		_explotionActive = false;
		return true;
	} // spawn

	//________________________________________________________________________

	bool CRocketController::accept(CMessage *message) {
		//Solamente podemos aceptar un contacto porque luego explotamos
		if(message->getMessageType() == Message::CONTACT_ENTER && !_explotionActive)
			return true;
		else
			return false;
	} // accept
	
	//________________________________________________________________________

	void CRocketController::process(CMessage *message) {
		switch(message->getMessageType()) {
			case Message::CONTACT_ENTER: {		
				CMessageContactEnter* contactMsg = static_cast<CMessageContactEnter*>(message);
				Logic::CEntity* playerHit = contactMsg->getEntity();
				if(playerHit!=_owner && !_explotionActive){
					// Los cohetes solos se destruyen al colisionar con algo que no sea yo
					// Este mensaje deberia recibirse al tocar cualquier otro
					// rigidbody del mundo
					_explotionActive=true;
					// Si es el escudo del screamer mandar directamente esos daños a la
					// entidad contra la que hemos golpeado (el escudo), sino, crear explosion
					if(playerHit->getType() == "ScreamerShield") {
						// Mandar mensaje de daño
						// Emitimos el mensaje de daño
						CMessageDamaged* dmgMsg = new CMessageDamaged();
						dmgMsg->setDamage(_explotionDamage);
						dmgMsg->setEnemy(_owner); // No tiene importancia en este caso
						playerHit->emitMessage(dmgMsg);

						// Crear efecto y sonido de absorcion

						// Eliminamos la entidad en diferido
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
					}
					else {
						// Eliminamos la entidad en diferido
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
						// Creamos la explosion
						createExplotion();

						//Sonido de explosion
						Logic::CMessageAudio *maudio=new Logic::CMessageAudio();
						maudio->setRuta(_audioExplotion);
						maudio->setId("audioExplotion");
						maudio->setPosition(_entity->getPosition());
						maudio->setNotIfPlay(false);
						maudio->setIsPlayer(false);
						_entity->emitMessage(maudio);
					}
				break;
				}
			}
		}
	} // process

	//________________________________________________________________________

	void CRocketController::createExplotion() {
		// EntitiesHit sera el buffer que contendra la lista de entidades que ha colisionado
		// con el overlap
		CEntity** entitiesHit = NULL;
		int nbHits = 0;

		// Hacemos una query de overlap con la geometria de una esfera en la posicion 
		// en la que se encuentra la granada con el radio que se indique de explosion
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_explotionRadius);
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, _entity->getPosition(), entitiesHit, nbHits);

		// Mandamos el mensaje de daño a cada una de las entidades que hayamos golpeado
		// Además aplicamos un desplazamiento al jugador 
		for(int i = 0; i < nbHits; ++i) {
			// Si la entidad golpeada es valida
			if(entitiesHit[i] != NULL) {
				// Emitimos el mensaje de daño
				CMessageDamaged* msg = new CMessageDamaged();
				msg->setDamage(_explotionDamage);
				msg->setEnemy(_owner);
				entitiesHit[i]->emitMessage(msg);

				// Emitimos el mensaje de desplazamiento por daños
				CMessageAddForcePlayer* msg2 = new CMessageAddForcePlayer();
				// Seteamos la fuerza y la velocidad
				msg2->setPower(0.1f);
				msg2->setVelocity(0.12f);
				// Seteamos el vector director del desplazamiento
				Vector3 direccionImpacto = entitiesHit[i]->getPosition() - _entity->getPosition();
				direccionImpacto.normalise();
				msg2->setDirection(direccionImpacto);
				entitiesHit[i]->emitMessage(msg2);
			}
		}

		if(nbHits > 0) delete [] entitiesHit;

		/*
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
		*/

		//Graphics::CParticle *particle = Graphics::CServer::getSingletonPtr()->getActiveScene()->createParticle(_entity->getName(),"ExplosionParticle", _entity->getPosition());

		CMessageCreateParticle *particle = new CMessageCreateParticle();
		particle->setParticle("SpawnParticle");
		//particle->setParticle("ExplosionParticle");
		particle->setPosition(_entity->getPosition());

		_entity->emitMessage(particle);

	} // createExplotion

	//________________________________________________________________________

	void CRocketController::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

	//________________________________________________________________________


} // namespace Logic

