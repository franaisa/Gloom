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

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageCollisionDown.h"
#include "Logic/Messages/MessageAvatarWalk.h"
#include "Logic/Messages/MessageMouse.h"
#include "Logic/Messages/MessageRebound.h"
#include "Logic/Messages/MessageAddForcePlayer.h"
#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageSide.h"

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

		if(entityInfo->hasAttribute("powerSideJump"))
			_powerSideJump = entityInfo->getFloatAttribute("powerSideJump");

		if(entityInfo->hasAttribute("sumSpeedSideJump"))
			_sumSpeedSideJump = entityInfo->getFloatAttribute("sumSpeedSideJump");

		if(entityInfo->hasAttribute("sumSpeedSideJumpConcat"))
			_sumSpeedSideJumpConcat = entityInfo->getFloatAttribute("sumSpeedSideJumpConcat");

		if(entityInfo->hasAttribute("maxTimeSideJump"))
			_maxTimeSideJump = entityInfo->getIntAttribute("maxTimeSideJump");

		if(entityInfo->hasAttribute("maxTimeConcatSideJump"))
			_maxTimeConcatSideJump = entityInfo->getIntAttribute("maxTimeConcatSideJump");

		if(entityInfo->hasAttribute("powerJump"))
			_powerJump = entityInfo->getFloatAttribute("powerJump");

		if(entityInfo->hasAttribute("restitutionMoveAir"))
			_restitutionMoveAir= entityInfo->getFloatAttribute("restitutionMoveAir");

		if(entityInfo->hasAttribute("gravity"))
			_gravity= entityInfo->getDoubleAttribute("gravity");

		if(entityInfo->hasAttribute("maxSpeedDown"))
			_maxSpeedDown= entityInfo->getFloatAttribute("maxSpeedDown");
		

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CAvatarController::activate()
	{
		IComponent::activate();

		//Desactivo movimientos para respawn
		_walking=false;
		_walkingBack=false;
		_strafingRight=false;
		_strafingLeft=false;
		_unpressLeft=false;
		_unpressRight=false;
		_jumping=false;

		//Inicializacion variables de control ( salto, salto lateral, concatenacion salto lateral y rebote )
		_jumping = false;
		_jumpingControl = false;
		_speedJump=-0.5f;
		_falling=true;
		_caida=false;

		_jumpLeft=0;
		_jumpRight=0;
		_timeSideJump=0;
		_sideJump=false;
		_velocitySideJump=false;
		_unpressRight=false;
		_unpressLeft=false;
		_readySideJumpLeft=false;
		_readySideJumpRight=false;
		_dontCountUntilUnpress=false;

		_nConcatSideJump=0;
		_timeConcatSideJump=0;
		_activeConcat=false;
		_sideFly=false;
		_sideContact=false;

		_rebound=false;
		_force=false;
		_applyForce=false;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate()
	{
		IComponent::deactivate();
	} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::accept(CMessage *message)
	{
		return message->getMessageType() == Message::CONTROL || 
			message->getMessageType() == Message::COLLISION_DOWN ||
			message->getMessageType() == Message::REBOUND ||
			message->getMessageType() == Message::ADDFORCEPLAYER ||
			message->getMessageType() == Message::CEALING ||
			message->getMessageType() == Message::SIDE;
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
			break;
		case Message::COLLISION_DOWN:
			_falling=((CMessageCollisionDown*)message)->getCollisionDown();
			break;
		case Message::REBOUND:
			_dirRebound=((CMessageRebound*)message)->getDir();
			rebound();
			break;
		case Message::ADDFORCEPLAYER:
			_powerJumpForce=((CMessageAddForcePlayer*)message)->getPower();
			_velocityForce=((CMessageAddForcePlayer*)message)->getVelocity();
			_directionForce=((CMessageAddForcePlayer*)message)->getDirection();
			force();
			break;
		case Message::CEALING:
			_speedJump=-0.02;
			break;
		case Message::SIDE:
			_direccionSaltoCaida=Vector3(0,0,0);
			break;
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
		
		
		CMessageSetAnimation * anim = new CMessageSetAnimation();
		anim->setString("Walk");
		anim->setBool(true);
		_entity->emitMessage(anim);
		_walking = true;
	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::walkBack() 
	{
		CMessageSetAnimation * anim = new CMessageSetAnimation();
		anim->setString("WalkBack");
		anim->setBool(true);
		_entity->emitMessage(anim, this);
		_walkingBack = true;
	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopWalk() 
	{
		CMessageSetAnimation * anim = new CMessageSetAnimation();
		anim->setString("Idle");
		anim->setBool(true);
		_entity->emitMessage(anim, this);
		_walking = false;
	} // stopWalk
	
		//---------------------------------------------------------

	void CAvatarController::stopWalkBack() 
	{
		CMessageSetAnimation * anim = new CMessageSetAnimation();
		anim->setString("Idle");
		anim->setBool(true);
		_entity->emitMessage(anim, this); 
		_walkingBack = false;
	} // stopWalk


	//---------------------------------------------------------

	void CAvatarController::strafeLeft() 
	{
		CMessageSetAnimation * anim = new CMessageSetAnimation();
		anim->setString("StrafeLeft");
		anim->setBool(true);
		_entity->emitMessage(anim);
		_strafingLeft = true;
	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::strafeRight() 
	{
		CMessageSetAnimation * anim = new CMessageSetAnimation();
		anim->setString("StrafeRight");
		anim->setBool(true);
		_entity->emitMessage(anim);
		_strafingRight = true;
	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopStrafeLeft() 
	{
		CMessageSetAnimation * anim = new CMessageSetAnimation();
		anim->setString("Idle");
		anim->setBool(true);
		_entity->emitMessage(anim, this);
		_strafingLeft = false;
		_unpressLeft = true;
	} // stopWalk

	//---------------------------------------------------------

		void CAvatarController::stopStrafeRight() 
	{
		
		CMessageSetAnimation * anim = new CMessageSetAnimation();
		anim->setString("Idle");
		anim->setBool(true);
		_entity->emitMessage(anim, this);
		_strafingRight = false;
		_unpressRight = true;
	} // stopWalk

	//---------------------------------------------------------

	void CAvatarController::jump()
	{
		_jumping = true;
	}//jump
	
	//---------------------------------------------------------

	void CAvatarController::rebound()
	{
		_rebound = true;
	}//rebound
	
	//---------------------------------------------------------

	void CAvatarController::force()
	{
		_force = true;
	}//rebound
	
	//---------------------------------------------------------
	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//Vector dirección que mandaremos a la física
		Vector3 direction(Vector3::ZERO);

		// Calculamos si hay vectores de dirección de avance y strafe,
		// hayamos la dirección de la suma y escalamos según la
		// velocidad y el tiempo transcurrido.

		//Control del movimiento sobre el plano (izquierda/derecha, delante, atrás)
		Vector3 directionWalk(Vector3::ZERO);
		Vector3 directionStrafe(Vector3::ZERO);

		//Delante/atrás
		if(_walking || _walkingBack)
		{
			directionWalk = Math::getDirection(_entity->getYaw());
			if(_walkingBack)
				directionWalk *= -1;
			//Anulacion forward/back
			if(_walking && _walkingBack)
				directionWalk *= 0;
		}
		direction+= directionWalk;
		//std::cout << "direccion en avatarcontroller " << direction << std::endl;

		//Control del tiempo para el salto lateral(al contar aqui cuando se activa no se cuenta el primer tick)
		if(_jumpLeft!=0 || _jumpRight!=0){
			_timeSideJump+=msecs;
		}
		else{
			_timeSideJump=0;
		}

		//Controlamos cuando soltamos la tecla para hacer que la siguiente vez se active el salto
		if(_unpressLeft && _jumpLeft==1 && !_dontCountUntilUnpress){
			_readySideJumpLeft=true;
			_readySideJumpRight=false;
			_jumpRight=0;
			_unpressLeft=false;
		}
		else if(_unpressRight && _jumpRight==1 && !_dontCountUntilUnpress){
			_readySideJumpRight=true;
			_readySideJumpLeft=false;
			_jumpLeft=0;
			_unpressRight=false;
		}
		//Cuando soltemos la segunda presión entonces empezamos el conteo de presiones otra vez
		else if((_unpressRight || _unpressLeft) && _dontCountUntilUnpress){
			_dontCountUntilUnpress=false;
			_unpressRight=false;
			_unpressLeft=false;
			_jumpRight=0;
			_jumpLeft=0;
			_readySideJumpLeft=false;
			_readySideJumpRight=false;
			_timeSideJump=0;
		}
		else if(_unpressRight || _unpressLeft){
			_unpressRight=false;
			_unpressLeft=false;
		}

		//Izquierda/Derecha
		if(_strafingLeft || _strafingRight)
		{
			//Si se presionaron ambas teclas
			if(_strafingRight && _strafingLeft){
				_timeSideJump=0;
				_jumpRight=0;
				_jumpLeft=0;
				_readySideJumpLeft=false;
				_readySideJumpRight=false;
			}
			//Si se presionó la izq , el contador esta a 0 y puedo contar || o el salto izq esta listo y le di a la izq
			else if((_strafingLeft && _jumpLeft==0 && !_dontCountUntilUnpress) || (_readySideJumpLeft && _strafingLeft)){
				if(_jumpRight!=0)
					_timeSideJump=0;
				_jumpLeft++;
				_jumpRight=0;
				_readySideJumpLeft=false;
				_readySideJumpRight=false;
			}
			//contrario al de arriba
			else if((_strafingRight  && _jumpRight==0 && !_dontCountUntilUnpress) || (_readySideJumpRight && _strafingRight)){
				if(_jumpLeft!=0)
					_timeSideJump=0;
				_jumpRight++;
				_jumpLeft=0;
				_readySideJumpLeft=false;
				_readySideJumpRight=false;
			}

			//Si se activo el salto lateral hacia algun lado y está dentro del tiempo
			if((_jumpRight==2 || _jumpLeft==2) && _timeSideJump<_maxTimeSideJump){ 
				_jumping=true; //Activo el salto
				_sideJump=true;
				_dontCountUntilUnpress=true;
				_nConcatSideJump++;
				_timeSideJump=0;
				_jumpRight=0;
				_jumpLeft=0;
				_readySideJumpRight=false;
				_readySideJumpLeft=false;
				//Control del salto lateral concatenado
				//Si llevamos mas de uno hecho,no estoy cayendo y el tiempo es inferior a _maxTimeConcatSideJump activamos la concatenacion (dará velocidad)
				if(_nConcatSideJump>1 && _timeConcatSideJump<_maxTimeConcatSideJump && !_falling){
					_activeConcat=true;
					_timeConcatSideJump=0;
					_sideContact=false;
				}
				//Si llevo al menos 1 salto, no estoy cayendo, pero el tiempo es mayor a 500msecs reseteo las variables de concatenacion de salto
				else if(_nConcatSideJump>1 && !_falling){
					_nConcatSideJump=1; // A uno por que seria un nuevo conteo de saltos laterales
					_timeConcatSideJump=0;
					_sideContact=false;
					_activeConcat=false;
				}
			}
			//Si se pasó el tiempo reseteo
			else if(_timeSideJump>_maxTimeSideJump){
				_timeSideJump=0;
				_jumpLeft=0;
				_jumpRight=0;
				_readySideJumpRight=false;
				_readySideJumpLeft=false;
			}
		
			directionStrafe = 
					Math::getDirection(_entity->getYaw() + Math::PI/2);
			if(_strafingRight)
				directionStrafe *= -1;
			//Para facilitar el strafe, anulación de strafes
			if(_strafingLeft && _strafingRight)
				directionStrafe *= 0;	
		}
		direction += directionStrafe;
		//std::cout << "direccion en avatarcontroller 2" << direction << std::endl;

		//Control del salto (normal y lateral)
		//El PhysicController nos envía por mensaje el atributo _falling (devuelve el del frame anterior) y asi sabemos si esta tocando el suelo y puedo saltar
		if(!_falling){
			_canJump=true;
			_speedJump=-0.5f;
			_jumpingControl=false;
			_caida=false;
			_velocitySideJump=false;
			_applyForce=false;
			//Caí luego activo el booleano para que empiece la cuenta de concatenacion
			if(_sideFly){
				_sideContact=true;
				_sideFly=false;
			}
			//Hubo salto lateral, cuando caiga diré que he caido y empezaré la cuenta de concatenacion
			if(_sideJump){
				_sideFly=true;
			}
		}
		else{
			_canJump=false;
			_sideJump=false;
			_jumping=false;
		}


		//Si active el rebote, activo un salto con una dirección en concreto (la que me pasaron en el mensaje rebote) y desactivo el rebote
		if(_rebound){
			_jumping=true;
			_canJump=true;
			direction=_dirRebound;
			_rebound=false;
		}
		//std::cout << "direccion en avatarcontroller 3" << direction << std::endl;

		//Aumento el tiempo de conteo para la concatenacion de saltos
		if(_sideContact){
			_timeConcatSideJump+=msecs;
		}

		//Gravedad
		direction+=Vector3(0,1,0);

		//Caida con MRUA (no puedo saltar, no estoy saltando y no estoy controlando la caida)
		if(!_canJump && !_jumpingControl && !_caida){
			_direccionSaltoCaida=direction;
			_caida=true;
			_speedJump=-0.02;
		}//if (!_canJump && !_jumpingControl)
		if(_caida){
			_speedJump=_speedJump + _gravity*msecs; //MRUA
			if(_speedJump<_maxSpeedDown)
				_speedJump=_maxSpeedDown;
		}


		//Calculamos la nueva velocidad si hay salto en ejecución
		if(_jumpingControl){
			_speedJump=_speedJump + _gravity*msecs; //MRUA
			if(_speedJump<_maxSpeedDown)
				_speedJump=_maxSpeedDown;
		}//if (_jumpingControl)

		//Si hay que aplicar el salto debido a saltos, saltos laterales, rebotes o jumpers
		if(_jumping && _canJump || _force ){
			//Si es una fuerza
			if(_force){
				_force=false;
				_speedJump=_powerJumpForce;
				_direccionSaltoCaida=_directionForce;
				_applyForce=true;
			}
			//Si es un salto normal
			else if(!_sideJump){
				_speedJump=_powerJump; // Velocidad explosiva inicial para el salto normal
				_direccionSaltoCaida=direction; //Guardamos la dirección del salto al iniciarse
			}
			//Sino es un salto lateral
			else{
				_speedJump=_powerSideJump; // Velocidad explosiva inicial para el salto lateral
				_velocitySideJump=true;
				_direccionSaltoCaida=direction; //Guardamos la dirección del salto al iniciarse
			}
			_jumpingControl=true;
		}//if (_jumping && _canJump)


		//Si estamos en un salto o cayendo hay que aplicar la dirección con la que se inició,
		//además hay que penalizar cualquier intento de movimiento en el aire, aplicaremos un factor de X(el factor por el que multiplicas el movimiento actual) sobre la dirección actual
		if(_jumpingControl || _caida){

			direction.x=_direccionSaltoCaida.x+direction.x*_restitutionMoveAir;
			direction.z=_direccionSaltoCaida.z+direction.z*_restitutionMoveAir;
		}
		std::cout << "direccion en avatarcontroller" << direction << std::endl;
		//Normalizamos y luego calculamos la magnitud correcta para la dirección (sin contar el salto/eje Y)
		//Vector3 directXZY=direction.normalisedCopy();
		//std::cout << "direccion en avatarcontroller 5" << directXZY << std::endl;
		//Si saltamos en un jumper nos desplazaremos mucho mas rapido (ignorando si veniamos de un salto lateral/concatenado)
		//Aplicaremos más velocidad lateral si se trata de un desplazamiento lateral (para desplazarnos más rápido, recorriendo más terreno)
		//Si ademas esta activa la concatenacion de saltos pues llegaremos mas lejos
		if(_applyForce){
			direction.x *= msecs * _velocityForce;
			direction.z *= msecs * _velocityForce;
		}
		else if(!_velocitySideJump){
			
			direction.x *= msecs * _speed;
			direction.z *= msecs * _speed;
			
		}
		else{
			if(_activeConcat){
				direction.x *= msecs * _speed * _sumSpeedSideJumpConcat;
				direction.z *= msecs * _speed * _sumSpeedSideJumpConcat;
			}
			else{

				direction.x *= msecs * _speed * _sumSpeedSideJump;
				direction.z *= msecs * _speed * _sumSpeedSideJump;
			}
		}


		//Calculamos el desplazamiento del salto y lo añadimos al vector que se mandará por mensaje
		direction.y *= msecs * _speedJump;
		//direction.y=direction.y;

		//std::cout << "direccion en avatarcontroller " << directXZY << std::endl;

		//Pasamos a la Física la dirección del movimiento para que se encargue ella de movernos
		Logic::CMessageAvatarWalk* m = new Logic::CMessageAvatarWalk();
		m->setDirection(direction);
		_entity->emitMessage(m);

	} // tick

} // namespace Logic

