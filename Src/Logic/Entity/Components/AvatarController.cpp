//---------------------------------------------------------------------------
// AvatarController.cpp
//---------------------------------------------------------------------------

/**
@file AvatarController.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CAvatarController
@see Logic::IComponent

@author Rubén Mulero Guerrero
@author Francisco Aisa García
@date Abril, 2013
*/

#include "AvatarController.h"
#include "CameraFeedbackNotifier.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Components/Camera.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Messages/MessageHudDebugData.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"
#include "Logic/Messages/MessageAddForcePlayer.h"
#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageStopAnimation.h"
#include "Logic/Messages/MessageHudDebugData.h"
#include "Logic/Messages/MessageChangeGravity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

#include <math.h>

namespace Logic {

	IMP_FACTORY(CAvatarController);

	//________________________________________________________________________

	CAvatarController::CAvatarController() : _gravity(Vector3::ZERO),
											 _touchingGround(false),
											 _cameraFX(NULL) {
		
		// Inicializamos el array que contiene los vectores
		// de cada tecla de movimiento
		initMovementCommands();
	}

	//________________________________________________________________________

	CAvatarController::~CAvatarController() {
		// Nada que hacer
	}

	//________________________________________________________________________

	bool CAvatarController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("acceleration") && "Error: No se ha definido el atributo acceleration en el mapa" );
		_acceleration = entityInfo->getFloatAttribute("acceleration");

		assert( entityInfo->hasAttribute("maxVelocity") && "Error: No se ha definido el atributo maxVelocity en el mapa" );
		_maxVelocity = entityInfo->getFloatAttribute("maxVelocity");
		_maxGravVelocity = _maxVelocity*6;

		assert( entityInfo->hasAttribute("frictionCoef") && "Error: No se ha definido el atributo frictionCoef en el mapa" );
		_frictionCoef = entityInfo->getFloatAttribute("frictionCoef");
		
		assert( entityInfo->hasAttribute("airFrictionCoef") && "Error: No se ha definido el atributo airFrictionCoef en el mapa" );
		_airFrictionCoef = entityInfo->getFloatAttribute("airFrictionCoef");

		assert( entityInfo->hasAttribute("airSpeedCoef") && "Error: No se ha definido el atributo airSpeedCoef en el mapa" );
		_airSpeedCoef = entityInfo->getFloatAttribute("airSpeedCoef");
		
		assert( entityInfo->hasAttribute("gravity") && "Error: No se ha definido el atributo gravity en el mapa" );
		_gravity.y = entityInfo->getFloatAttribute("gravity");

		assert( entityInfo->hasAttribute("jumpForce") && "Error: No se ha definido el atributo jumpForce en el mapa" );
		_jumpForce = entityInfo->getFloatAttribute("jumpForce");

		assert( entityInfo->hasAttribute("dodgeForce") && "Error: No se ha definido el atributo dodgeForce en el mapa" );
		_dodgeForce = entityInfo->getVector3Attribute("dodgeForce");

		return true;
	} // spawn

	//________________________________________________________________________

	void CAvatarController::onActivate() {
		_displacementDir = _momentum = Vector3::ZERO;
	} // activate

	//________________________________________________________________________

	bool CAvatarController::accept(const std::shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::CONTROL			||
			   msgType == Message::ADDFORCEPLAYER;
	} // accept

	//________________________________________________________________________

	void CAvatarController::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				ControlType commandType = std::static_pointer_cast<CMessageControl>(message)->getType();

				// Comando de movimiento
				if(commandType >=0 && commandType < MAX_MOVEMENT_COMMANDS) {
					executeMovementCommand(commandType);
				}
				// Comando de salto
				else if(commandType == Control::JUMP) {
					executeJump();
				}
				// Comando de esquiva
				else if(commandType > Control::JUMP && commandType < Control::MOUSE) {
					executeDodge(commandType);
				}
				// Comando de raton
				else if(commandType == Control::MOUSE) {
					std::shared_ptr<CMessageMouse> mouseMsg = std::static_pointer_cast<CMessageMouse>(message);
					mouse( mouseMsg->getMouse() );
				}

				break;
			}
			case Message::ADDFORCEPLAYER:{
				//Fuerza aplicada al jugador
				std::shared_ptr<CMessageAddForcePlayer> addForceMsg = std::static_pointer_cast<CMessageAddForcePlayer>(message);
				addForce( addForceMsg->getForce() );
				break;
			}
			case Message::CHANGE_GRAVITY:{
				_gravity = std::static_pointer_cast<CMessageChangeGravity>(message)->getGravity();
			}
		}

	} // process	

	//________________________________________________________________________

	void CAvatarController::mouse(float XYturn[]) {
		_entity->setYawPitch(XYturn[0],XYturn[1]);
		
		//Ñapeado deberia de estar en el componente camera.cpp , aunque ojo son dependientes lo mismo aqui no esta mal
		//Hay que pensar sobre ello
		//Si la camara se ejecuta despues de esto vamos retardeds
		//Graphics::CCamera *camera=_entity->getMap()->getScene()->getCamera();
		//camera->moveCamera(Ogre::Radian(XYturn[0]),Ogre::Radian(XYturn[1]));

		
	} // turn

	//________________________________________________________________________

	void CAvatarController::onStart() {
		// Para evitar overhead de mensajes nos quedamos con el puntero al 
		// componente CPhysicController que es el que realmente se encargará 
		// de desplazar al controlador del jugador.
		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");
		assert(_physicController && "Error: El player no tiene un controlador fisico");

		_cameraFX = _entity->getComponent<CCameraFeedbackNotifier>("CCameraFeedbackNotifier");
	}

	//________________________________________________________________________

	void CAvatarController::onFixedTick(unsigned int msecs) {
		// Calculamos el vector de desplazamiento teniendo en cuenta
		// si estamos en el aire o en el suelo
		Vector3 displacement = _touchingGround ? estimateGroundMotion(msecs) : estimateAirMotion(msecs);

		// Seteamos el efecto de camara
		setCameraEffect();
		
		// Tratamos de mover el controlador fisico con el desplazamiento estimado.
		// En caso de colision, el controlador fisico nos informa.
		// Debido al reposicionamiento de la cápsula que hace PhysX, le seteamos un offset fijo
		// al movernos para asegurarnos de que hay colision
		Vector3 oldPosition = _entity->getPosition();
		manageCollisions( _physicController->move(displacement-Vector3(0,0.15f,0), msecs), oldPosition );
	} // tick

	//________________________________________________________________________

	void CAvatarController::setCameraEffect() {
		if(_cameraFX == NULL) return; // Por si se trata de un remote player

		if(_touchingGround && _displacementDir != Vector3::ZERO) {
			// Efecto de andar
			_cameraFX->playerIsWalking(true, _displacementDir.x);
		}
		else {
			// Parar efecto de andar
			_cameraFX->playerIsWalking(false);
		}
	}

	//________________________________________________________________________

	void CAvatarController::manageCollisions(unsigned collisionFlags, Vector3 oldPosition) {
		if(_touchingGround = collisionFlags & Physics::eCOLLISION_DOWN) {
			// Colision con los pies detectada
			if(_cameraFX != NULL)
				_cameraFX->playerIsTouchingGround(_momentum.y);
		}
		
		if(collisionFlags & Physics::eCOLLISION_UP) {
			// Colisión con la cabeza
			_momentum.y = 0;
		}

		if(collisionFlags & Physics::eCOLLISION_SIDES){
			//necesitamos la posicion anterior del personaje, para ver la dirección 
			//y la velocidad a la que nos hemos movido, y calculamos el momentum
			//resultado de habernos movido y haber chocado contra las paredes
			Vector3 newMom = _entity->getPosition() - oldPosition;
			_momentum.x=newMom.x;
			_momentum.z=newMom.z;

			if(_cameraFX != NULL)
				_cameraFX->playerIsSideColliding( true, _momentum.length() );
		}
		else {
			if(_cameraFX != NULL)
				_cameraFX->playerIsSideColliding(false, 0);
		}

	}

	//________________________________________________________________________

	Vector3 CAvatarController::estimateMotionDirection(const Vector3& direction) {
		// Si nuestro movimiento es nulo no hacemos nada
		if(direction == Vector3::ZERO) return Vector3::ZERO;

		// Mediante trigonometria basica sacamos el angulo que forma el desplazamiento
		// que queremos llevar a cabo
		float displacementYaw = asin(direction.normalisedCopy().x);
		// Obtenemos una copia de la matriz de transformación del personaje
		Matrix4 characterTransform = _entity->getTransform();
		// Si estamos andando hacia atras, invertimos el giro
		if(direction.z < 0) displacementYaw *= -1;

		// Rotamos la matriz de transformacion tantos grados como diga el vector 
		// desplazamiento calculado de pulsar las teclas
		Math::yaw(displacementYaw, characterTransform);
		// Obtenemos el vector unitario de orientación de la matriz de transformación
		Vector3 motionDirection = Math::getDirection(characterTransform);
		// Invertimos el vector resultante si nos estamos desplazando hacia atras
		// porque el yaw se calcula de forma contraria al andar hacia atras
		if(direction.z < 0) motionDirection *= -1;

		// Devolvemos la dirección del movimiento estimado
		return motionDirection;
	}

	//________________________________________________________________________

	Vector3 CAvatarController::estimateGroundMotion(unsigned int msecs) {
		// Si no nos queremos desplazar en ninguna dirección aplicamos el coeficiente
		// de rozamiento
		float coef = (_displacementDir == Vector3::ZERO) ? 0.8f : _maxVelocity/(_maxVelocity+(0.5*_acceleration*msecs));
		
		/*
		if(_displacementDir != Vector3::ZERO){
		    std::shared_ptr<CMessageHudDebugData> m = std::make_shared<CMessageHudDebugData>();
		    m->setKey("incremento");
		    m->setValue(_momentum);
		    _entity->emitMessage(m);
		
		}
		*/
		// Aumentamos el desplazamiento en la dirección dada en función de la aceleración
		// y el tiempo transcurrido -> s = u · t + 1/2 · a · t^2

		///////////////////////////////////////////////////////////////
		////////////////////////// DEBUG MODE /////////////////////////
		///////////////////////////////////////////////////////////////
		if(_displacementDir != Vector3::ZERO){
			++ticks;
		}else{
			if(ticks>0)
				std::cout << "el avatar controller se ha ejecutado " << ticks << " ticks" << std::endl;
			ticks=0;
		}
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////

		_momentum += Vector3(1,0,1) * estimateMotionDirection(_displacementDir) * _acceleration * msecs * 0.5f;
		// Multiplicamos la inercia por el coeficiente de rozamiento (para decelerarla)
		_momentum *= coef;
		// Seteamos una gravedad fija para que la cápsula colisione contra el suelo
		_momentum.y = _gravity.y * msecs;

		return _momentum;
	}

	//________________________________________________________________________

	Vector3 CAvatarController::estimateAirMotion(unsigned int msecs) {
		// Aplicamos una fuerza de rozamiento al desplazamiento aereo para frenarnos
		// un poco al estar volando
		_momentum *= Vector3(_airFrictionCoef, 1, _airFrictionCoef);
		// Calculamos el coeficiente de movimiento aereo (que debería ser más reducido
		// que el terrestre) teniendo en cuenta los milisegundos transcurridos
		float speedCoef = _airSpeedCoef / (double)msecs;

		// Aumentamos el desplazamiento en la dirección dada teniendo en cuenta
		// que nos movemos más lento en el aire -> -> s = u · t + 1/2 · a · t^2
		_momentum += Vector3(speedCoef, 0, speedCoef) * estimateMotionDirection(_displacementDir) * _acceleration * msecs * 0.5f;
		// Incrementamos el empuje de la gravedad
		_momentum.y += _gravity.y * msecs * 0.5f;

		// Fijamos la velocidad de caida a la velocidad máxima de caida si es necesario
		normalizeGravity();

		return _momentum;
	}

	//________________________________________________________________________

	void CAvatarController::normalizeGravity() {
		// Normalizamos la velocidad vertical
		float gVelocity = (_momentum*Vector3(0,1,0)).length();
		if(gVelocity >_maxGravVelocity){
			double coef = _maxGravVelocity/gVelocity;
			_momentum*=Vector3(1,coef,1);
		}
	}

	//________________________________________________________________________

	void CAvatarController::executeMovementCommand(ControlType commandType) {
		Vector3 dir = _movementCommands[commandType];

		//seteamos la animacion correcta con el resultado de las teclas pulsadas
		//stopAnimation(dir);

		_displacementDir += dir;

		executeAnimation(dir);
	}

	//________________________________________________________________________

	void CAvatarController::initMovementCommands() {
		// Comandos de movimiento
		_movementCommands[Control::WALK]				= Vector3(0,0,1);
		_movementCommands[Control::WALKBACK]			= Vector3(0,0,-1);
		_movementCommands[Control::STRAFE_LEFT]			= Vector3(1,0,0);
		_movementCommands[Control::STRAFE_RIGHT]		= Vector3(-1,0,0);
		_movementCommands[Control::STOP_WALK]			= Vector3(0,0,-1);
		_movementCommands[Control::STOP_WALKBACK]		= Vector3(0,0,1);
		_movementCommands[Control::STOP_STRAFE_LEFT]	= Vector3(-1,0,0);
		_movementCommands[Control::STOP_STRAFE_RIGHT]	= Vector3(1,0,0);
		
		// Comandos de salto
		_movementCommands[Control::JUMP]				= Vector3(0,1,0);
		_movementCommands[Control::SIDEJUMP_LEFT]		= Vector3(1,0,0);
		_movementCommands[Control::SIDEJUMP_RIGHT]		= Vector3(-1,0,0);
		_movementCommands[Control::DODGE_BACKWARDS]		= Vector3(0,0,-1);
		_movementCommands[Control::DODGE_FORWARD]		= Vector3(0,0,1);

	}

	//________________________________________________________________________

	void CAvatarController::executeJump(){
		if(_touchingGround) {
			_momentum.y = _jumpForce;
			_touchingGround = false;
		}
	}

	//________________________________________________________________________

	void CAvatarController::addForce(const Vector3 &force){
		_momentum+=force;
		if(_momentum.y>0 && _touchingGround)
			_touchingGround=false;
	}

	//________________________________________________________________________

	void CAvatarController::executeDodge(ControlType commandType){
		_displacementDir += _movementCommands[commandType];
		if(!_touchingGround)
			return;
		Vector3 dir = estimateMotionDirection(_movementCommands[commandType])+Vector3(0,1,0);
		addForce(dir.normalisedCopy()*_dodgeForce);
	}

	//________________________________________________________________________

	void CAvatarController::executeAnimation(Vector3 dir){

		std::shared_ptr<CMessageSetAnimation> anim = std::make_shared<CMessageSetAnimation>();
		
		if(dir.x!=0){
			if(_displacementDir.x==0){
				anim->setString("Idle");
				anim->setBool(true);
			}else if(_displacementDir.x==1){
				anim->setString("StrafeLeft");
				anim->setBool(true);
			}else if(_displacementDir.x==-1){
				anim->setString("StrafeRight");
				anim->setBool(true);
			}
		}else if(dir.z!=0){
			if(_displacementDir.z==0){
				anim->setString("Idle");
				anim->setBool(true);
			}else if(_displacementDir.z==1){
				anim->setString("Walk");
				anim->setBool(true);
			}else if(_displacementDir.z==-1){
				anim->setString("WalkBack");
				anim->setBool(true);
			}
		}
		_entity->emitMessage(anim, this); 
	}

	//________________________________________________________________________

	void CAvatarController::stopAnimation(Vector3 dir){
		//primero paramos la animación que este corriendo en sentido contrario a lo que hacemos
		std::shared_ptr<CMessageSetAnimation> stopAnim = std::make_shared<CMessageSetAnimation>();

		if(_displacementDir == Vector3::ZERO){
			stopAnim->setString("Idle");
		}else{
			if(dir.x!= _displacementDir.x){
				if (_displacementDir.x==0)
					return;
				else if(dir.x == -1)
					stopAnim->setString("StrafeLeft");
				else
					stopAnim->setString("StrafeRight");
			}else if(dir.z!= _displacementDir.z){
				if (_displacementDir.z==0)
					return;
				else if(dir.z == -1)
					stopAnim->setString("Walk");
				else
					stopAnim->setString("WalkBack");
			}
		}
		_entity->emitMessage(stopAnim, this); 
	}
	//________________________________________________________________________

} // namespace Logic
