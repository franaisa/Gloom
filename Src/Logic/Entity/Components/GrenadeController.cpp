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
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageCreateParticle.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

namespace Logic {
	
	IMP_FACTORY(CGrenadeController);

	//________________________________________________________________________

	CGrenadeController::CGrenadeController() : _timer(0) { 
		// Nada que hacer
	}
	
	//________________________________________________________________________

	void CGrenadeController::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		// Actualizamos el timer. Si se ha cumplido el tiempo limite de explosion
		// eliminamos la entidad granada y creamos la entidad explosion.
		_timer += msecs;
		if(_timer > _explotionTime) {
			// Eliminamos la entidad en diferido
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
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
		_audioExplotion = entityInfo->getStringAttribute("explotionAudio");
		_explotionActive = false;

		return true;
	} // spawn

	//________________________________________________________________________

	bool CGrenadeController::accept(CMessage *message) {
		//Solamente podemos aceptar un contacto porque luego explotamos
		if(message->getMessageType() == Message::CONTACT_ENTER && !_explotionActive){
			_explotionActive=true;
			return true;
		}
		else
			return false;
	} // accept
	
	//________________________________________________________________________

	void CGrenadeController::process(CMessage *message) {
		switch( message->getMessageType() ) {
			case Message::CONTACT_ENTER: {
				CMessageContactEnter* contactMsg = static_cast<CMessageContactEnter*>(message);
				Logic::CEntity* playerHit = contactMsg->getEntity();
			
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
				}

				break;
			}
		}
	} // process

	//________________________________________________________________________

	void CGrenadeController::createExplotion() {
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
			// y no se trata del escudo
			if(entitiesHit[i] != NULL && entitiesHit[i]->getType() != "ScreamerShield") {
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

		// Limpiamos el buffer si es necesario
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

		//Sonido de explosion
		Logic::CMessageAudio *maudio=new Logic::CMessageAudio();
		maudio->setRuta(_audioExplotion);
		maudio->setId("audioExplotion");
		maudio->setPosition(_entity->getPosition());
		maudio->setNotIfPlay(false);
		maudio->setIsPlayer(false);
		_entity->emitMessage(maudio);

		CMessageCreateParticle *particle = new CMessageCreateParticle();
		particle->setParticle("ExplosionParticle");
		particle->setPosition(_entity->getPosition());

		_entity->emitMessage(particle);
	} // createExplotion

	//________________________________________________________________________

	void CGrenadeController::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

} // namespace Logic

