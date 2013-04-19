/**
@file AvatarController.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CAvatarController
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Enero, 2013
*/

#include "AvatarController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/GameNetMsgManager.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageCollisionDown.h"
#include "Logic/Messages/MessageAvatarWalk.h"
#include "Logic/Messages/MessageMouse.h"
#include "Logic/Messages/MessageRebound.h"
#include "Logic/Messages/MessageAddForcePlayer.h"
#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageSide.h"
#include "Logic/Messages/MessageAudio.h"

#include "Audio.h"

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
		
		if(entityInfo->hasAttribute("audioStep"))
			_audioStep= entityInfo->getStringAttribute("audioStep");

		if(entityInfo->hasAttribute("audioJump"))
			_audioJump= entityInfo->getStringAttribute("audioJump");

		if(entityInfo->hasAttribute("audioSideJump"))
			_audioSideJump= entityInfo->getStringAttribute("audioSideJump");

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
		_jumping=false;
		//Inicializacion variables de control ( salto, salto lateral, concatenacion salto lateral y rebote )
		_jumping = false;
		_jumpingControl = false;
		_speedJump=-0.5f;
		_falling=false;
		_caida=false;
		_sideJump=false;
		_velocitySideJump=false;
		_activeConcat=false;
		_rebound=false;
		_force=false;
		_applyForce=false;

		_nConcatSideJump=0;
		_timeConcatSideJump=0;
		_sideFly=false;
		_sideContact=false;
		_timeMrua=0;

	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate()
	{
		IComponent::deactivate();
	} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::accept(const std::shared_ptr<CMessage>& message)
	{
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::CONTROL || 
			   msgType == Message::COLLISION_DOWN ||
			   msgType == Message::REBOUND ||
			   msgType == Message::ADDFORCEPLAYER ||
			   msgType == Message::CEALING ||
			   msgType == Message::SIDE;
	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(const std::shared_ptr<CMessage>& message)
	{
		//std::cout << "menssaje de avatar controller recibido" << std::endl;
		switch(message->getMessageType()) {
			case Message::CONTROL: {
				std::shared_ptr<CMessageControl> controlMsg = std::static_pointer_cast<CMessageControl>(message);
				Logic::ControlType controlType = controlMsg->getType();

				if(controlType==Control::WALK)
					walk();
				else if(controlType==Control::WALKBACK)
					walkBack();
				else if(controlType==Control::STOP_WALK)
					stopWalk();
				else if(controlType==Control::STOP_WALKBACK)
					stopWalkBack();
				else if(controlType==Control::STRAFE_LEFT)
					strafeLeft();
				else if(controlType==Control::STRAFE_RIGHT)
					strafeRight();
				else if(controlType==Control::STOP_STRAFE_LEFT)
					stopStrafeLeft();
				else if(controlType==Control::STOP_STRAFE_RIGHT)
					stopStrafeRight();
				else if(controlType==Control::MOUSE)
					mouse(std::static_pointer_cast<CMessageMouse>(controlMsg)->getMouse());
				else if(controlType==Control::JUMP)
					jump();
				else if(controlType==Control::SIDEJUMP_LEFT){
					strafeLeft();
					_nConcatSideJump++;
					_sideJump=true;
					_jumping=true;
				}
				else if(controlType==Control::SIDEJUMP_RIGHT){
					strafeRight();
					_nConcatSideJump++;
					_sideJump=true;
					_jumping=true;
				}
				break;
			}
			case Message::COLLISION_DOWN: {
				std::shared_ptr<CMessageCollisionDown> collDownMsg = std::static_pointer_cast<CMessageCollisionDown>(message);
				_falling=collDownMsg->getCollisionDown();
				break;
			}
			case Message::REBOUND: {
				std::shared_ptr<CMessageRebound> reboundMsg = std::static_pointer_cast<CMessageRebound>(message);
				_dirRebound=reboundMsg->getDir();
				rebound();
				break;
			}
			case Message::ADDFORCEPLAYER: {
				std::shared_ptr<CMessageAddForcePlayer> addForcePlayerMsg = std::static_pointer_cast<CMessageAddForcePlayer>(message);
				_powerJumpForce=addForcePlayerMsg->getPower();
				_velocityForce=addForcePlayerMsg->getVelocity();
				_directionForce=addForcePlayerMsg->getDirection();
				force();
				break;
			}
			case Message::CEALING: {
				_vo=-0.02;
				break;
			}
			case Message::SIDE: {
				_direccionSaltoCaida=Vector3(0,0,0);
				break;
			}
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
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setString("Walk");
		anim->setBool(true);
		_entity->emitMessage(anim);
		_walking = true;
	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::walkBack() 
	{
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setString("WalkBack");
		anim->setBool(true);
		_entity->emitMessage(anim, this);
		_walkingBack = true;
	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopWalk() 
	{
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setString("Idle");
		anim->setBool(true);
		_entity->emitMessage(anim, this);
		_walking = false;
	} // stopWalk
	
		//---------------------------------------------------------

	void CAvatarController::stopWalkBack() 
	{
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setString("Idle");
		anim->setBool(true);
		_entity->emitMessage(anim, this); 
		_walkingBack = false;
	} // stopWalk


	//---------------------------------------------------------

	void CAvatarController::strafeLeft() 
	{
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setString("StrafeLeft");
		anim->setBool(true);
		_entity->emitMessage(anim);
		_strafingLeft = true;
	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::strafeRight() 
	{
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setString("StrafeRight");
		anim->setBool(true);
		_entity->emitMessage(anim);
		_strafingRight = true;
	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopStrafeLeft() 
	{
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setString("Idle");
		anim->setBool(true);
		_entity->emitMessage(anim, this);
		_strafingLeft = false;

	} // stopWalk

	//---------------------------------------------------------

		void CAvatarController::stopStrafeRight() 
	{
		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		anim->setString("Idle");
		anim->setBool(true);
		_entity->emitMessage(anim, this);
		_strafingRight = false;

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
		//if(_entity->getName().compare("David")==0)
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

		//Izquierda/Derecha
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

		//Control de concatenacion de saltos (siempre y cuando hubo salto lateral)
		//Si hubo salto lateral, si llevamos mas de uno hecho,no estoy cayendo y el tiempo es inferior a _maxTimeConcatSideJump activamos la concatenacion (dará velocidad)
		if(_sideJump && _nConcatSideJump>1 && _timeConcatSideJump<_maxTimeConcatSideJump && !_falling){
			_activeConcat=true;
			_timeConcatSideJump=0;
			_sideContact=false;
		}	
		//Si llevo al menos 1 salto, no estoy cayendo, pero el tiempo es mayor a 500msecs reseteo las variables de concatenacion de salto
		else if(_sideJump && _nConcatSideJump>1 && !_falling){
			_nConcatSideJump=1; // A uno por que seria un nuevo conteo de saltos laterales
			_timeConcatSideJump=0;
			_sideContact=false;
			_activeConcat=false;
		}

		//Aumento el tiempo de conteo para la concatenacion de saltos
		if(_sideContact){
			_timeConcatSideJump+=msecs;
		}


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

		//Gravedad
		direction+=Vector3(0,1,0);

		//Caida con MRUA (no puedo saltar, no estoy saltando y no estoy controlando la caida)
		if(!_canJump && !_jumpingControl && !_caida){
			_direccionSaltoCaida=direction;
			_caida=true;
			_speedJump=-0.02;
		}//if (!_canJump && !_jumpingControl)
		if(_caida){
			_timeMrua+=msecs;
			_speedJump=_vo + _gravity*_timeMrua; //MRUA
			if(_speedJump<_maxSpeedDown)
				_speedJump=_maxSpeedDown;
		}


		//Calculamos la nueva velocidad si hay salto en ejecución
		if(_jumpingControl){
			_timeMrua+=msecs;
			_speedJump=_vo + _gravity*_timeMrua; //MRUA
			if(_speedJump<_maxSpeedDown)
				_speedJump=_maxSpeedDown;
		}//if (_jumpingControl)

		//Si hay que aplicar el salto debido a saltos, saltos laterales, rebotes o jumpers/misiles/fuerzas
		if(_jumping && _canJump || _force ){
			_timeMrua=0;
			//Si es una fuerza
			if(_force){
				_force=false;
				_vo=_powerJumpForce;
				_speedJump=_powerJumpForce;
				_direccionSaltoCaida=_directionForce;
				_applyForce=true;
			}
			//Si es un salto normal
			else if(!_sideJump){
				_vo=_powerJump;
				_speedJump=_powerJump; // Velocidad explosiva inicial para el salto normal
				_direccionSaltoCaida=direction; //Guardamos la dirección del salto al iniciarse
				//Sonido salto normal
				if(!Net::CManager::getSingletonPtr()->imServer()){
					std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
					audioMsg->setRuta(_audioJump);
					audioMsg->setId("Jump");
					audioMsg->setPosition(_entity->getPosition());
					audioMsg->setNotIfPlay(false);
					audioMsg->setIsPlayer(_entity->isPlayer());
					_entity->emitMessage(audioMsg);
				}
			}
			//Sino es un salto lateral
			else{
				_vo=_powerJump;
				_speedJump=_powerSideJump; // Velocidad explosiva inicial para el salto lateral
				_velocitySideJump=true;
				_direccionSaltoCaida=direction; //Guardamos la dirección del salto al iniciarse
				//Sonido salto lateral
				if(!Net::CManager::getSingletonPtr()->imServer()){
					std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
					audioMsg->setRuta(_audioSideJump);
					audioMsg->setId("sideJump");
					audioMsg->setPosition(_entity->getPosition());
					audioMsg->setNotIfPlay(false);
					audioMsg->setIsPlayer(_entity->isPlayer());
					_entity->emitMessage(audioMsg);
				}
			}
			_jumpingControl=true;
		}//if (_jumping && _canJump)


		//Si estamos en un salto o cayendo hay que aplicar la dirección con la que se inició,
		//además hay que penalizar cualquier intento de movimiento en el aire, aplicaremos un factor de X(el factor por el que multiplicas el movimiento actual) sobre la dirección actual
		if(_jumpingControl || _caida){
			direction.x=_direccionSaltoCaida.x+direction.x*_restitutionMoveAir;
			direction.z=_direccionSaltoCaida.z+direction.z*_restitutionMoveAir;
		}

		//Normalizamos y luego calculamos la magnitud correcta para la dirección (sin contar el salto/eje Y)
		Vector3 directXZY=direction.normalisedCopy();
		//Si saltamos en un jumper nos desplazaremos mucho mas rapido (ignorando si veniamos de un salto lateral/concatenado)
		//Aplicaremos más velocidad lateral si se trata de un desplazamiento lateral (para desplazarnos más rápido, recorriendo más terreno)
		//Si ademas esta activa la concatenacion de saltos pues llegaremos mas lejos
		if(_applyForce){
			directXZY.x *= msecs * _velocityForce;
			directXZY.z *= msecs * _velocityForce;
		}
		else if(!_velocitySideJump){
			directXZY.x *= msecs * _speed;
			directXZY.z *= msecs * _speed;
		}
		else{
			if(_activeConcat){
				directXZY.x *= msecs * _speed * _sumSpeedSideJumpConcat;
				directXZY.z *= msecs * _speed * _sumSpeedSideJumpConcat;
			}
			else{
				directXZY.x *= msecs * _speed * _sumSpeedSideJump;
				directXZY.z *= msecs * _speed * _sumSpeedSideJump;
			}
		}

		
		//Calculamos el desplazamiento del salto y lo añadimos al vector que se mandará por mensaje
		direction.y *= msecs * _speedJump;
		directXZY.y=direction.y;
		//std::cout << _falling << std::endl;
		//Pasamos a la Física la dirección del movimiento para que se encargue ella de movernos
		std::shared_ptr<CMessageAvatarWalk> m = std::make_shared<CMessageAvatarWalk>();
		m->setDirection(directXZY);
		_entity->emitMessage(m);

		//Audio
		//Si nos estamos moviendo pero no saltando/ni cayendo mandamos un mensaje de audio (pasos)
		if(!Net::CManager::getSingletonPtr()->imServer()){
			if((directXZY.x!=0 || directXZY.z!=0) && !_jumpingControl && !_caida){
				std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
				audioMsg->setRuta(_audioStep);
				audioMsg->setId(_entity->getEntityID()+"steps");
				audioMsg->setPosition(_entity->getPosition());
				audioMsg->setNotIfPlay(true);
				audioMsg->setIsPlayer(_entity->isPlayer());
				_entity->emitMessage(audioMsg);
			}
		}

	} // tick


} // namespace Logic

