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
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::accept(CMessage *message)
	{
		return message->getMessageType() == Message::CONTROL;
	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::CONTROL:
			if(((CMessageControl*)message)->getType()==Control::WALK)
				walk();
			else if(((CMessageControl*)message)->getType()==Control::WALKBACK)
				walkBack();
			else if(((CMessageControl*)message)->getType()==Control::STOP_WALK)
				stopWalk();
			else if(((CMessageControl*)message)->getType()==Control::STOP_WALKBACK)
				stopWalkBack();
			else if(((CMessageControl*)message)->getType()==Control::STRAFE_LEFT)
				strafeLeft();
			else if(((CMessageControl*)message)->getType()==Control::STRAFE_RIGHT)
				strafeRight();
			else if(((CMessageControl*)message)->getType()==Control::STOP_STRAFE_LEFT)
				stopStrafeLeft();
			else if(((CMessageControl*)message)->getType()==Control::STOP_STRAFE_RIGHT)
				stopStrafeRight();
			else if(((CMessageControl*)message)->getType()==Control::MOUSE)
				mouse(((CMessageMouse*)message)->getMouse());
			else if(((CMessageControl*)message)->getType()==Control::JUMP)
				jump();
		}

	} // process
	
	//---------------------------------------------------------

	void CAvatarController::mouse(const float* amount) 
	{
		_entity->yaw(amount[0]);
		_entity->pitch(amount[1]);

	} // turn
	
	//---------------------------------------------------------

	void CAvatarController::walk()
	{
		_walking = true;

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::walkBack() 
	{
		_walkingBack = true;

	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopWalk() 
	{
		_walking = false;

	} // stopWalk
	
		//---------------------------------------------------------

	void CAvatarController::stopWalkBack() 
	{
		 _walkingBack = false;

	} // stopWalk


	//---------------------------------------------------------

	void CAvatarController::strafeLeft() 
	{
		_strafingLeft = true;

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::strafeRight() 
	{
		_strafingRight = true;

	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopStrafeLeft() 
	{
		_strafingLeft = false;

	} // stopWalk

	//---------------------------------------------------------

		void CAvatarController::stopStrafeRight() 
	{
		_strafingRight = false;

	} // stopWalk

	//---------------------------------------------------------

	void CAvatarController::jump(){
		
		_jumping = true;
		std::cout << "FUNCION SALTO DE AVATARCONTROLLER";
	}//jump
	
	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//Vector dirección que mandaremos a la física
		Vector3 direction(Vector3::ZERO);
		//Si se presionó el salto lo mandaremos a la física para que se encargue de la simulación completa
		if(_jumping){
			direction+=Vector3(0,1,0);
			_jumping=false;
		}//if (_jumping)

		// Si nos estamos desplazando calculamos la próxima posición
		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.
		if(_walking || _walkingBack || _strafingLeft || _strafingRight)
		{
			Vector3 directionStrafe(Vector3::ZERO);

			if(_walking || _walkingBack)
			{
				direction += Math::getDirection(_entity->getYaw());
				if(_walkingBack)
					direction *= -1;
				//Anulacion forward/back
				if(_walking && _walkingBack)
					direction *= 0;
			}

			if(_strafingLeft || _strafingRight)
			{
				directionStrafe = 
						Math::getDirection(_entity->getYaw() + Math::PI/2);
				if(_strafingRight)
					directionStrafe *= -1;
				//Para facilitar el strafe, anulación de strafes
				if(_strafingLeft && _strafingRight)
					directionStrafe *= 0;
			}
			direction += directionStrafe;

			//Normalizamos y luego calculamos la magnitud correcta para la dirección
			direction.normalise();
			direction *= msecs * _speed;

			//Pasamos a la Física la dirección del movimiento para que se encargue ella de movernos
			Logic::CMessageAvatarWalk *m=new Logic::CMessageAvatarWalk(Logic::Message::AVATAR_WALK);
			m->setDirection(direction);
			_entity->emitMessage(m);
		}

	} // tick

} // namespace Logic

