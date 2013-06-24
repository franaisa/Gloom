//---------------------------------------------------------------------------
// AnimationManager.cpp
//---------------------------------------------------------------------------

/**
@file AnimationManager.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::AnimationManager
@see Logic::IComponent

@author Rubén Mulero Guerrero
@date June, 2013
*/

#include "AnimationManager.h"

#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageStopAnimation.h"
#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/MessageControl.h"

#include "Logic/Server.h"

#include "Logic/Maps/Map.h"

#include "AvatarController.h"

namespace Logic{
	IMP_FACTORY(CAnimationManager);
	CAnimationManager::CAnimationManager(): _flying(false){
		_lastDisplacementAnimation=Vector3::ZERO;

		
	}

	bool CAnimationManager::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		_avatarController =  _entity->getComponent<CAvatarController>("CAvatarController");
	}

	void CAnimationManager::onStart(){
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setLoop(true);
		anim->setAnimation("idle");
		anim->setExclude(false);
		_entity->emitMessage(anim);
	}

	void CAnimationManager::onFixedTick(unsigned int msecs){
		Vector3 displacementDir = _avatarController->getDisplacementDir();
		bool flying = _avatarController->getFlying();
		if(_lastDisplacementAnimation != displacementDir && !flying){
			_flying = false;
			_lastDisplacementAnimation = displacementDir;
			std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
			anim->setLoop(true);
			anim->setAnimation(getMotionAnimation(displacementDir));
			anim->setExclude(false);
			_entity->emitMessage(anim);
		}
		else if ( flying && !_flying ) {
			_flying = true;
			std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
			anim->setLoop(true);
			anim->setAnimation("airwalk");
			anim->setExclude(false);
			_entity->emitMessage(anim);
		}
		else if ( !flying && _flying ) {
			_flying = false;
			std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
			anim->setLoop(true);
			anim->setAnimation(getMotionAnimation(_lastDisplacementAnimation));
			anim->setExclude(false);
			_entity->emitMessage(anim);
		}
	}

	bool CAnimationManager::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::PLAYER_DEAD				||
			   msgType == Message::DAMAGED					||
			   msgType == Message::CONTROL;

	} // accept
	
	//---------------------------------------------------------

	void CAnimationManager::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PLAYER_DEAD:
				sendDeadMessage(std::static_pointer_cast<CMessagePlayerDead>(message)->getKiller());
				break;

			case Message::DAMAGED:
				//nada de momento
				break;

			case Message::CONTROL:
				ControlType ctrlType = std::static_pointer_cast<CMessageControl>(message)->getType();

				if( ( ctrlType == ControlType::JUMP || ctrlType == ControlType::DODGE_BACKWARDS || ctrlType == ControlType::DODGE_FORWARD || 
				ctrlType == ControlType::DODGE_LEFT || ctrlType == ControlType::DODGE_RIGHT ) ){

					std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
					anim->setLoop(false);
					anim->setAnimation("jump");
					anim->setExclude(true);
					_entity->emitMessage(anim);
				}
				break;

		}
	}

	void CAnimationManager::sendDeadMessage(TEntityID killer){
		CEntity * entity = CServer::getSingletonPtr()->getMap()->getEntityByID(killer);
		if(!entity)
			return;

		Vector3 direction = _entity->getPosition() - entity->getPosition();

		float angle = direction.normalisedCopy().angleBetween(Math::getDirection(entity->getOrientation()).normalisedCopy()).valueDegrees();

		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();

		//si no lo esta mirando la muerte ha sido por detras, luego ponemos
		//la animacion de muerte por detras
		if(angle < 90){
			
			anim->setLoop(false);
			anim->setAnimation("deadback");
		}else{
		//esta por delante, ponemos la animacion de muerte por delante
			anim->setLoop(false);
			anim->setAnimation("headshot");
		}
		anim->setExclude(true);
		_entity->emitMessage(anim);
	}

	std::string CAnimationManager::getMotionAnimation(const Vector3 &displacementDir){
		if(displacementDir == Vector3 (0,0,0) )		return	"idle";
		if(displacementDir == Vector3 (0,0,1) )		return	"forward";
		if(displacementDir == Vector3 (1,0,1) )		return	"forwardstrafeleft";
		if(displacementDir == Vector3 (-1,0,1))		return	"forwardstraferight";
		if(displacementDir == Vector3 (0,0,-1))		return	"back";
		if(displacementDir == Vector3 (1,0,-1))		return	"backstrafeleft";
		if(displacementDir == Vector3 (-1,0,-1))	return	"backstraferight";
		if(displacementDir == Vector3 (1,0,0) )		return	"strafeleft";
		if(displacementDir == Vector3 (-1,0,0))		return	"straferightminigun";

		//por si las moscas
		return "idle";
	}

}