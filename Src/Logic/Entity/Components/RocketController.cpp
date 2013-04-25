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
#include "Logic/Maps/Map.h"

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

	CRocketController::CRocketController() { 
		// Nada que hacer
	}


	void CRocketController::tick(unsigned int msecs){
	/*	inicio+=msecs;
		if(inicio>=1600 && contamosparada==false){
			inicio1=_entity->getPosition();
			contamosparada=true;
		}
		if(contamosparada)
			parada+=msecs;
		if(parada>=1600){
			parada1=_entity->getPosition();
			float distance=parada1.distance(inicio1);
			std::cout << "La distancia recorrida es : " << distance << std::endl;
			std::cout << "Cada 16msecs recorre: " << distance*100/1600;
			parada=0;
			contamosparada=false;
		}
		*/
	}

	//________________________________________________________________________

	bool CRocketController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		_explotionDamage = entityInfo->getFloatAttribute("explotionDamage");
		_explotionRadius = entityInfo->getFloatAttribute("explotionRadius");
		_audioExplotion = entityInfo->getStringAttribute("explotionAudio");
		_explotionActive = false;
		inicio=parada=0;
		contamosparada=false;
		return true;
	} // spawn

	//________________________________________________________________________

	bool CRocketController::accept(const std::shared_ptr<CMessage>& message) {
		//Solamente podemos aceptar un contacto porque luego explotamos
		return message->getMessageType() == Message::CONTACT_ENTER && !_explotionActive ||
			message->getMessageType() == Message::CONTACT_EXIT && !_explotionActive;
	} // accept
	
	//________________________________________________________________________

	void CRocketController::process(const std::shared_ptr<CMessage>& message) {
		if(!_explotionActive) {
			switch(message->getMessageType()) {
				case Message::CONTACT_ENTER: {
					std::shared_ptr<CMessageContactEnter> contactMsg = std::static_pointer_cast<CMessageContactEnter>(message);
					Logic::TEntityID idPlayerHit = contactMsg->getEntity();
					CEntity * playerHit = CServer::getSingletonPtr()->getMap()->getEntityByID(idPlayerHit);

					// Los cohetes solos se destruyen al colisionar con algo que no sea yo
					// Este mensaje deberia recibirse al tocar cualquier otro
					// rigidbody del mundo
					_explotionActive=true;
					// Si es el escudo del screamer mandar directamente esos daños a la
					// entidad contra la que hemos golpeado (el escudo), sino, crear explosion
					if(playerHit->getType() == "ScreamerShield") {
						// Mandar mensaje de daño
						// Emitimos el mensaje de daño
						std::shared_ptr<CMessageDamaged> dmgMsg = std::make_shared<CMessageDamaged>();
						dmgMsg->setDamage(_explotionDamage);
						dmgMsg->setEnemy(_owner); // No tiene importancia en este caso
						playerHit->emitMessage(dmgMsg);

						// Crear efecto y sonido de absorcion

						// Eliminamos la entidad en diferido
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity,false);
					}
					else {
						// Eliminamos la entidad en diferido
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity,false);
						// Creamos la explosion
						createExplotion();	
					}
					break;	
				}
				case Message::CONTACT_EXIT: {
						std::cout << "CONTACT EXIT DEL COHETE, no deberia entrar nunca" << std::endl;
				}
			}//switch(message->getMessageType())
		}//	if(!_explotionActive)
	} // process

	//________________________________________________________________________

	void CRocketController::createExplotion() {
		//std::cout << "CREO LA EXPLOSION EN LA POSICION : " << _entity->getPosition() << std::endl;
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
				std::shared_ptr<CMessageDamaged> dmgMsg = std::make_shared<CMessageDamaged>();
				dmgMsg->setDamage(_explotionDamage);
				dmgMsg->setEnemy(_owner);
				entitiesHit[i]->emitMessage(dmgMsg);

				// Emitimos el mensaje de desplazamiento por daños
				std::shared_ptr<CMessageAddForcePlayer> addForcePlayerMsg = std::make_shared<CMessageAddForcePlayer>();
				// Seteamos la fuerza y la velocidad
				addForcePlayerMsg->setPower(0.1f);
				addForcePlayerMsg->setVelocity(0.12f);
				// Seteamos el vector director del desplazamiento
				Vector3 direccionImpacto = entitiesHit[i]->getPosition() - _entity->getPosition();
				direccionImpacto.normalise();
				addForcePlayerMsg->setDirection(direccionImpacto);
				entitiesHit[i]->emitMessage(addForcePlayerMsg);
			}
		}

		if(nbHits > 0) delete [] entitiesHit;

		//Solo para singlePlayer, quitar al terminar
		//Sonido de explosion
		std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
		audioMsg->setRuta(_audioExplotion);
		audioMsg->setId("audioExplotion");
		audioMsg->setPosition(_entity->getPosition());
		audioMsg->setNotIfPlay(false);
		audioMsg->setIsPlayer(false);
		_entity->emitMessage(audioMsg);

		std::shared_ptr<CMessageCreateParticle> particleMsg = std::make_shared<CMessageCreateParticle>();
		particleMsg->setParticle("ExplosionParticle");
		particleMsg->setPosition(_entity->getPosition());
		_entity->emitMessage(particleMsg);

	} // createExplotion

	//________________________________________________________________________

	void CRocketController::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

	//________________________________________________________________________


} // namespace Logic

