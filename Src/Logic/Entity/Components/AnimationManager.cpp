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
		initAnimationCommands();

		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setLoop(true);
		anim->setAnimation("idle");
		anim->setExclude(false);
		_entity->emitMessage(anim);
	}

	void CAnimationManager::onFixedTick(unsigned int msecs){
		Vector3 displacementDir = _avatarController->getDisplacementDir();
		bool flying = _avatarController->getFlying();
		if(_lastDisplacementAnimation != displacementDir/* && !flying*/){

			auto animation = _animations.find(getMotionAnimation(displacementDir));

			_flying = false;
			_lastDisplacementAnimation = displacementDir;
			std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
			anim->setLoop(true);
			anim->setAnimation(animation->second.animation);
			anim->setExclude(false);
			anim->setRewind(animation->second.rewind);
			_entity->emitMessage(anim);
		}
		/*else if ( flying && !_flying ) {
			_flying = true;
			std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
			anim->setLoop(true);
			anim->setAnimation("airwalk");
			anim->setExclude(false);
			_entity->emitMessage(anim);
		}*/
		/*else if ( !flying && _flying ) {
			_flying = false;
			std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
			anim->setLoop(true);
			anim->setAnimation(getMotionAnimation(_lastDisplacementAnimation));
			anim->setRewind(1);
			anim->setExclude(false);
			_entity->emitMessage(anim);
		}*/
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
			case Message::PLAYER_DEAD: {
				std::shared_ptr<CMessagePlayerDead> playerDeadMsg = std::static_pointer_cast<CMessagePlayerDead>(message);
				CEntity* killer = playerDeadMsg->getKiller();

				sendDeadMessage(killer);
				break;
			}
			case Message::DAMAGED: {
				//nada de momento
				break;
			}
			case Message::CONTROL: {
				ControlType ctrlType = std::static_pointer_cast<CMessageControl>(message)->getType();

				if( ( ctrlType == ControlType::JUMP || ctrlType == ControlType::DODGE_BACKWARDS || ctrlType == ControlType::DODGE_FORWARD || 
				ctrlType == ControlType::DODGE_LEFT || ctrlType == ControlType::DODGE_RIGHT ) ){

					std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
					anim->setLoop(false);
					anim->setAnimation("jump");
					anim->setExclude(true);
					anim->setRewind(1);
					_entity->emitMessage(anim);
				}
				break;
			}
		}
	}

	void CAnimationManager::sendDeadMessage(CEntity* killer){
		if(!killer)
			return;

		Vector3 direction = _entity->getPosition() - killer->getPosition();
		direction.normalise();
		Vector3 entityDirection = killer->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
		entityDirection.normalise();
		float angle = direction.angleBetween(entityDirection).valueDegrees();

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

		//por si las moscas
		return "idle";
	}

	void CAnimationManager::initAnimationCommands(){
		TAnimation anim;
		anim.second.animation = "idle";
		anim.second.rewind = 1;
		anim.first = "idle";
		_animations.insert(anim);
		
		anim.second.animation = "forward";
		anim.second.rewind = 1;
		anim.first = "forward";
		_animations.insert(anim);

		anim.second.animation = "strafe_right";
		anim.second.rewind = -1;
		anim.first = "forwardstrafeleft";
		_animations.insert(anim);

		anim.second.animation = "strafe_right";
		anim.second.rewind = 1;
		anim.first = "forwardstraferight";
		_animations.insert(anim);

		anim.second.animation = "forward";
		anim.second.rewind = -1;
		anim.first = "back";
		_animations.insert(anim);

		anim.second.animation = "strafe_right";
		anim.second.rewind = -1;
		anim.first = "backstrafeleft";
		_animations.insert(anim);

		anim.second.animation = "strafe_right";
		anim.second.rewind = 1;
		anim.first = "backstraferight";
		_animations.insert(anim);

		anim.second.animation = "strafe_right";
		anim.second.rewind = -1;
		anim.first = "strafeleft";
		_animations.insert(anim);

		anim.second.animation = "strafe_right";
		anim.second.rewind = 1;
		anim.first = "straferight";
		_animations.insert(anim);
	}

}