/**
@file RocketControllerClient.cpp

@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Abril, 2013
*/

#include "RocketControllerClient.h"

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

#include "Map/MapEntity.h"

namespace Logic {
	
	IMP_FACTORY(CRocketControllerClient);

	CRocketControllerClient::CRocketControllerClient(){ 
		// Nada que hacer
	}

	//________________________________________________________________________

	bool CRocketControllerClient::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		_audioExplotion = entityInfo->getStringAttribute("explotionAudio");
		_explotionActive = false;
		return true;
	} // spawn

	//________________________________________________________________________

	bool CRocketControllerClient::accept(const std::shared_ptr<CMessage>& message) {
		//Solamente podemos aceptar un contacto porque luego explotamos
		return message->getMessageType() == Message::CONTACT_ENTER && !_explotionActive ||
			message->getMessageType() == Message::CONTACT_EXIT && !_explotionActive;
	} // accept
	
	//________________________________________________________________________

	void CRocketControllerClient::process(const std::shared_ptr<CMessage>& message) {
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

	void CRocketControllerClient::createExplotion() {
		//Sonido de explosion
		std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
		audioMsg->setRuta(_audioExplotion);
		audioMsg->setId("audioExplotion");
		audioMsg->setPosition(_entity->getPosition());
		audioMsg->setNotIfPlay(false);
		audioMsg->setIsPlayer(false);
		_entity->emitMessage(audioMsg);
		//Particulas
		std::shared_ptr<CMessageCreateParticle> particleMsg = std::make_shared<CMessageCreateParticle>();
		particleMsg->setParticle("ExplosionParticle");
		particleMsg->setPosition(_entity->getPosition());
		_entity->emitMessage(particleMsg);
	} // createExplotion

	//________________________________________________________________________


} // namespace Logic

