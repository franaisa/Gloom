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
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Components/Camera.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"
#include "Logic/Messages/MessageAddForcePlayer.h"

namespace Logic {

	IMP_FACTORY(CAvatarController);

	//________________________________________________________________________

	CAvatarController::CAvatarController() : _gravity(Vector3::ZERO),
											 _touchingGround(false) {
		
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

	void CAvatarController::activate() {
		IComponent::activate(); // Necesario para el onStart
		_displacementDir = _momentum = Vector3::ZERO;
	} // activate

	//________________________________________________________________________

	bool CAvatarController::accept(CMessage *message) {
		return message->getMessageType() == Message::CONTROL ||
			message->getMessageType() == Message::ADDFORCEPLAYER;
	} // accept

	//________________________________________________________________________

	
	void CAvatarController::process(CMessage *message) {
		switch( message->getMessageType() )
		{
			case Message::CONTROL: {
				ControlType commandType = static_cast<CMessageControl*>(message)->getType();

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
					CMessageMouse* mouseMsg = static_cast<CMessageMouse*>(message);
					mouse( mouseMsg->getMouse() );
				}

				break;
			}
			case Message::ADDFORCEPLAYER:{
				//megahipertetrahackeado porque el mensaje addforce este es una mierda como una puta catedral
				addForce(static_cast<CMessageAddForcePlayer*>(message)->getForce());
				break;
			}

							
		}

	} // process	

	//________________________________________________________________________

	void CAvatarController::mouse(float XYturn[]) {
		_entity->yaw(XYturn[0]);
		_entity->pitch(XYturn[1]);
	} // turn

	//________________________________________________________________________

	void CAvatarController::onStart(unsigned int msecs) {
		IComponent::onStart(msecs);

		// Para evitar overhead de mensajes nos quedamos con el puntero al 
		// componente CPhysicController que es el que realmente se encargará 
		// de desplazar al controlador del jugador.
		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");
		assert(_physicController && "Error: El player no tiene un controlador fisico");
	}

	//________________________________________________________________________

	void CAvatarController::tick(unsigned int msecs) {
		//@deprecated
		IComponent::tick(msecs);

		// Calculamos el vector de desplazamiento teniendo en cuenta
		// si estamos en el aire o en el suelo
		Vector3 displacement = _touchingGround ? estimateGroundMotion(msecs) : estimateAirMotion(msecs);
		
		// Tratamos de mover el controlador fisico con el desplazamiento estimado.
		// En caso de colision, el controlador fisico nos informa.
		// Debido al reposicionamiento de la cápsula que hace PhysX, le seteamos un offset fijo
		// al movernos para asegurarnos de que hay colision
		manageCollisions( _physicController->move(displacement-Vector3(0,0.15f,0), msecs) );
	} // tick

	//________________________________________________________________________

	void CAvatarController::manageCollisions(unsigned collisionFlags) {
		// Colision con los pies detectada
		_touchingGround = collisionFlags & Physics::eCOLLISION_DOWN;
		


		if(collisionFlags & Physics::eCOLLISION_UP) {
			// Colisión con la cabeza
			_momentum.y = 0;
		}

		if(collisionFlags & Physics::eCOLLISION_SIDES){
			//necesitamos la posicion anterior del personaje, para ver la dirección 
			//y la velocidad a la que nos hemos movido
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
		float coef = (_displacementDir == Vector3::ZERO) ? 0.8f : _maxVelocity/(_maxVelocity+(0.5*_acceleration*msecs));;
		
		// Multiplicamos la inercia por el coeficiente de rozamiento (para decelerarla)

		//std::cout << "suelo" << std::endl;
		
		// Aumentamos el desplazamiento en la dirección dada en función de la aceleración
		// y el tiempo transcurrido -> s = u · t + 1/2 · a · t^2
		_momentum += Vector3(1,0,1) * estimateMotionDirection(_displacementDir) * _acceleration * msecs * 0.5f;
		// Seteamos una gravedad fija para que la cápsula colisione contra el suelo
		_momentum *= coef;
		
		_momentum.y = _gravity.y * msecs;

		// Fijamos la velocidad de movimiento a la velocidad máxima si es necesario
		//normalizeDirection();
		
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
		std::cout << _momentum << std::endl;

		_momentum += Vector3(speedCoef, 0, speedCoef) * estimateMotionDirection(_displacementDir) * _acceleration * msecs * 0.5f;
		// Incrementamos el empuje de la gravedad
		_momentum.y += _gravity.y * msecs * 0.5f;

		//std::cout << (_momentum*Vector3(1,0,1)).length() << " vel max " << _maxVelocity << std::endl;

		// Fijamos la velocidad de movimiento a la velocidad máxima si es necesario
		//normalizeDirection();
		// Fijamos la velocidad de caida a la velocidad máxima de caida si es necesario
		normalizeGravity();

		return _momentum;
	}

	//________________________________________________________________________

	void CAvatarController::normalizeDirection() {
		// Normalizamos la velocidad horizontal
		float momVelocity = (_momentum*Vector3(1,0,1)).length();
		if(momVelocity >_maxVelocity){
			double coef = _maxVelocity/momVelocity;
			_momentum*=Vector3(coef,1,coef);
			
		}
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
		_displacementDir += _movementCommands[commandType];
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

} // namespace Logic
