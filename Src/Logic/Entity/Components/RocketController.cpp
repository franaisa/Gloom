/**
@file RocketController.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "RocketController.h"
#include "PhysicDynamicEntity.h"

#include "Physics/Server.h"
#include "Physics/RaycastHit.h"

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

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic {
	
	IMP_FACTORY(CRocketController);

	CRocketController::CRocketController() : _temporal(0) { 
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

		// Desactivamos el cohete para que no intervenga en la query
		CPhysicDynamicEntity* comp;
		if(comp = _entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity")) {
			comp->deactivateSimulation();
		}

		// Hacemos una query de overlap con la geometria de una esfera en la posicion 
		// en la que se encuentra la granada con el radio que se indique de explosion
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_explotionRadius);
		Vector3 explotionPos = _entity->getPosition();
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, explotionPos, entitiesHit, nbHits);

		// Mandamos el mensaje de daño a cada una de las entidades que hayamos golpeado
		// Además aplicamos un desplazamiento al jugador 
		for(int i = 0; i < nbHits; ++i) {
			// Si la entidad golpeada es valida
			if(entitiesHit[i] != NULL) {
				// Comprobamos el punto de contacto
				Vector3 rayDirection = entitiesHit[i]->getPosition();

				// Tiramos el rayo al centro de la capsula del enemigo
				// No podemos tirar el rayo directamente porque se lo tira a los pies y a veces no hay contacto
				// Como solucion rapida (aehem.. ñapa) voy a sacar la info de la altura del screamer
				// que teoricamente es la misma que la de todos los players
				Map::CEntity * info = Logic::CEntityFactory::getSingletonPtr()->getInfo("Screamer");
				if(info != NULL) rayDirection.y += info->getIntAttribute("physic_height");
				rayDirection = rayDirection - explotionPos;
				
				// Hacemos una query de raycast desde el punto de explosion hasta la entidad analizada
				// de la query de overlap (para saber el punto de colision).
				// No es ni lo mas exacto ni lo mas eficiente, pero soluciona la papeleta.
				Ogre::Ray ray( explotionPos, rayDirection.normalisedCopy() );
				int bufferSize;
				Physics::CRaycastHit* hitBuffer;
				Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _explotionRadius, hitBuffer, bufferSize);

				// Calculamos el daño en base a la distancia del golpe
				float dmg = 0;
				for(int k = 0; k < bufferSize; ++k) {
					if(hitBuffer[k].entity == entitiesHit[i]) {
						dmg = _explotionDamage * ( 1 - (hitBuffer[k].distance/_explotionRadius) );
					}
				}

				if(bufferSize > 0) delete [] hitBuffer;

				// Emitimos el mensaje de daño
				std::shared_ptr<CMessageDamaged> dmgMsg = std::make_shared<CMessageDamaged>();
				dmgMsg->setDamage(dmg);
				dmgMsg->setEnemy(_owner);
				entitiesHit[i]->emitMessage(dmgMsg);

				// Emitimos el mensaje de desplazamiento por daños
				std::shared_ptr<CMessageAddForcePlayer> addForcePlayerMsg = std::make_shared<CMessageAddForcePlayer>();
				// Seteamos la fuerza y la velocidad

				// Seteamos el vector director del desplazamiento
				/*Vector3 direccionImpacto = entitiesHit[i]->getPosition() - _entity->getPosition();
				direccionImpacto.normalise();

				msg2->setForce(direccionImpacto*_force);
				entitiesHit[i]->emitMessage(msg2);*/
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

	// Dibujado de raycast para depurar
	void CRocketController::drawRaycast(const Ray& raycast, float dist) {
		Graphics::CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		Ogre::SceneManager *mSceneMgr = scene->getSceneMgr();

		std::stringstream aux;
		aux << "laser" << _temporal++;
		std::string laser = aux.str();

		Ogre::ManualObject* myManualObject =  mSceneMgr->createManualObject(laser); 
		Ogre::SceneNode* myManualObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(laser+"_node"); 
 
		myManualObject->begin("laser", Ogre::RenderOperation::OT_LINE_STRIP);
		Vector3 v = raycast.getOrigin();
		myManualObject->position(v.x,v.y,v.z);

		for(int i=0; i < dist;++i){
			Vector3 v = raycast.getPoint(i);
			myManualObject->position(v.x,v.y,v.z);
			// etc 
		}

		myManualObject->end(); 
		myManualObjectNode->attachObject(myManualObject);
	}// drawRaycast

} // namespace Logic

