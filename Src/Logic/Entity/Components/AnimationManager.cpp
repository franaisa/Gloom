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

#include "AvatarController.h"

namespace Logic{
	IMP_FACTORY(CAnimationManager);
	CAnimationManager::CAnimationManager(): _flying(false){
		_lastDisplacementAnimation=Vector3::ZERO;

		//iniciamos las animaciones por desplazamiento en tierra
		_displacementAnims.insert( TAnim(Vector3(0,0,0) ,	"idle"));
		_displacementAnims.insert( TAnim(Vector3(0,0,1) ,	"forward"));
		_displacementAnims.insert( TAnim(Vector3(1,0,1) ,	"forwardstrafeleft"));
		_displacementAnims.insert( TAnim(Vector3(-1,0,1),	"forwardstraferight"));
		_displacementAnims.insert( TAnim(Vector3(0,0,-1),	"back"));
		_displacementAnims.insert( TAnim(Vector3(1,0,-1),	"backstrafeleft"));
		_displacementAnims.insert( TAnim(Vector3(-1,0,-1),	"backstraferight"));
		_displacementAnims.insert( TAnim(Vector3(1,0,0),	"strafeleft"));
		_displacementAnims.insert( TAnim(Vector3(-1,0,0),	"straferight"));
	}

	bool CAnimationManager::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		_avatarController =  _entity->getComponent<CAvatarController>("CAvatarController");
	}

	void CAnimationManager::onStart(){
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setLoop(true);
		anim->setAnimation("idle");
		_entity->emitMessage(anim);
	}

	void CAnimationManager::onFixedTick(unsigned int msecs){
		Vector3 displacementDir = _avatarController->getDisplacementDir();
		bool flying = _avatarController->getFlying();
		if(_lastDisplacementAnimation != displacementDir && !flying){
			_lastDisplacementAnimation = displacementDir;
			std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
			anim->setLoop(true);
			anim->setAnimation(_displacementAnims[_lastDisplacementAnimation]);
			_entity->emitMessage(anim);
		}
		else if ( flying && !_flying ) {
			_flying = true;
			std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
			anim->setLoop(true);
			anim->setAnimation("airwalk");
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
				ctrlType == ControlType::DODGE_LEFT || ctrlType == ControlType::DODGE_RIGHT ) && !_flying ){

					std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
					anim->setLoop(true);
					anim->setAnimation("jump");
					_entity->emitMessage(anim);
				}
				break;

		}
	}

	void CAnimationManager::sendDeadMessage(TEntityID killer){

	}
}