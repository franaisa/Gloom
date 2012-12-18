/**
@file AvatarController.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "AvatarController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CAvatarController);
	
	//---------------------------------------------------------

	bool CAvatarController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		if(entityInfo->hasAttribute("speed"))
			_speed = entityInfo->getFloatAttribute("speed");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CAvatarController::activate()
	{
		_jumping = false;
		timeJump = 0.0;
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::accept(const TMessage &message)
	{
		return message._type == Message::CONTROL;

	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::CONTROL:
			if(!message._string.compare("walk"))
				walk();
			else if(!message._string.compare("walkBack"))
				walkBack();
			else if(!message._string.compare("stopWalk"))
				stopWalk();
			else if(!message._string.compare("stopWalkBack"))
				stopWalkBack();
			else if(!message._string.compare("strafeLeft"))
				strafeLeft();
			else if(!message._string.compare("strafeRight"))
				strafeRight();
			else if(!message._string.compare("stopStrafeLeft"))
				stopStrafeLeft();
			else if(!message._string.compare("stopStrafeRight"))
				stopStrafeRight();
			else if(!message._string.compare("jump"))
				jump();
			else if(!message._string.compare("turn"))
				turn(message._float);
		}

	} // process
	
	//---------------------------------------------------------

	void CAvatarController::turn(float amount) 
	{
		_entity->yaw(amount);

	} // turn
	
	//---------------------------------------------------------

	void CAvatarController::walk() 
	{
		_walking = true;

		// Cambiamos la animación
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = "Walk";
		message._bool = true;
		_entity->emitMessage(message,this);

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::walkBack() 
	{
		_walkingBack = true;

		// Cambiamos la animación
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = "WalkBack";
		message._bool = true;
		_entity->emitMessage(message,this);

	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopWalk() 
	{
		_walking = false;

		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		if(!(_strafingLeft || _strafingRight))
		{
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
		}

	} // stopWalk
	
		//---------------------------------------------------------

	void CAvatarController::stopWalkBack() 
	{
		 _walkingBack = false;

		// Cambiamos la animación si no seguimos desplazándonos
		// lateralmente
		if(!(_strafingLeft || _strafingRight))
		{
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
		}

	} // stopWalk


	//---------------------------------------------------------

	void CAvatarController::strafeLeft() 
	{
		_strafingLeft = true;

		// Cambiamos la animación
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = "StrafeLeft";
		message._bool = true;
		_entity->emitMessage(message,this);

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::strafeRight() 
	{
		_strafingRight = true;

		// Cambiamos la animación
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = "StrafeRight";
		message._bool = true;
		_entity->emitMessage(message,this);

	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopStrafeLeft() 
	{
		_strafingLeft = false;

		// Cambiamos la animación si no seguimos andando
		if(!(_walking || _walkingBack))
		{
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
		}

	} // stopWalk

	//---------------------------------------------------------

		void CAvatarController::stopStrafeRight() 
	{
		_strafingRight = false;
		// Cambiamos la animación si no seguimos andando
		if(!(_walking || _walkingBack))
		{
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
		}

	} // stopWalk

	//---------------------------------------------------------

	void CAvatarController::jump(){
		//si ya estaba saltando, pasamos
		if(_jumping)
			return;

		_jumping = true;
	}//jump
	
	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//si estamos saltando, realizamos una simulación de un salto (provisional hasta tener physX
		if(_jumping){
			if(timeJump > 800){
				timeJump = 0.0;
				_jumping = false;
			}else if (timeJump >=400){
				Vector3 direction(0,-1,0);
				direction *= msecs * 0.05;
				timeJump+=msecs;
				_entity->setPosition(_entity->getPosition()+direction);
			}else{
				Vector3 direction(0,1,0);
				direction *= msecs * 0.05;
				timeJump+=msecs;
				_entity->setPosition(_entity->getPosition()+direction);
			}
		}//if (_jumping)

		// Si nos estamos desplazando calculamos la próxima posición
		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.
		if(_walking || _walkingBack || _strafingLeft || _strafingRight)
		{
			Vector3 direction(Vector3::ZERO);
			Vector3 directionStrafe(Vector3::ZERO);

			if(_walking || _walkingBack)
			{
				direction = Math::getDirection(_entity->getYaw());
				if(_walkingBack)
					direction *= -1;
				if(_walking && _walkingBack)
					direction *= 0;
			}

			if(_strafingLeft || _strafingRight)
			{
				directionStrafe = 
						Math::getDirection(_entity->getYaw() + Math::PI/2);
				if(_strafingRight)
					directionStrafe *= -1;
				//Para facilitar el strafe
				if(_strafingLeft && _strafingRight)
					directionStrafe *= 0;
			}

			direction += directionStrafe;
			direction.normalise();
			direction *= msecs * _speed;
			Vector3 newPosition = _entity->getPosition() + direction;
			_entity->setPosition(newPosition);
		}

	} // tick

} // namespace Logic

