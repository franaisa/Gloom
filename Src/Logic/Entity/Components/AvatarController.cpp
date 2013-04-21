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

namespace Logic {
	IMP_FACTORY(CAvatarController);

	CAvatarController::CAvatarController() : _gravity(0, -9.81f, 0),
											 _touchingGround(false) {
		// Inicializamos el array que contiene los vectores
		// de cada tecla de movimiento
		initMovementCommands();
	}
	
	//________________________________________________________________________

	bool CAvatarController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;
		
		assert( entityInfo->hasAttribute("acceleration") && "Error: No se ha definido el atributo acceleration en el mapa" );
		_acceleration = entityInfo->getFloatAttribute("acceleration");

		//assert( entityInfo->hasAttribute("maxVelocity") && "Error: No se ha definido el atributo maxVelocity en el mapa" );
		//_maxVelocity = entityInfo->getFloatAttribute("maxVelocity");

		return true;

	} // spawn
	
	//________________________________________________________________________

	void CAvatarController::activate() {
		IComponent::activate(); // Necesario para el onStart
		_displacement = _momentum = Vector3::ZERO;
	} // activate
	
	//________________________________________________________________________

	void CAvatarController::deactivate() {
		IComponent::deactivate();
	} // deactivate
	
	//________________________________________________________________________

	bool CAvatarController::accept(CMessage *message) {
		return message->getMessageType() == Message::CONTROL;
	} // accept
	
	//________________________________________________________________________

	void CAvatarController::process(CMessage *message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				ControlType commandType = static_cast<CMessageControl*>(message)->getType();

				// Comando de movimiento
				if(commandType >=0 && commandType < 8) {
					executeMovementCommand(commandType);
				}
				// Comando de raton
				else if(commandType == Control::MOUSE) {
					CMessageMouse* mouseMsg = static_cast<CMessageMouse*>(message);
					mouse( mouseMsg->getMouse() );
				}

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

		// Nos quedamos con el puntero al componente CPhysicController
		// que es el que realmente se encargará de desplazar al controlador
		// del jugador.
		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");
		assert(_physicController && "Error: El player no tiene un controlador fisico");
	}

	//________________________________________________________________________

	void CAvatarController::tick(unsigned int msecs) {
		//@deprecated
		IComponent::tick(msecs);

		// Calculamos el vector de movimiento del personaje
		std::cout << msecs << std::endl;
		_momentum*=10.0f/(double)msecs;
		_momentum+= _touchingGround ? estimateGroundMotion(msecs) : estimateAirMotion(msecs);

		// Tratamos de mover el controlador fisico con el movimiento estimado
		// en caso de colision, el controlador fisico nos informará.
		
		float momVelocity = _momentum.length();
		if(momVelocity >_maxVelocity){
			double coef = _maxVelocity/momVelocity;
			_momentum*=coef;
		}

		manageCollisions( _physicController->move(_momentum, msecs) );
	} // tick

	//________________________________________________________________________

	void CAvatarController::manageCollisions(unsigned collisionFlags) {
		// En función de los flags de colisión que se disparen, informamos al
		// componente que corresponda (en este caso avatarController)

		_touchingGround = collisionFlags & Physics::eCOLLISION_DOWN;
		
		/*if(collisionFlags & Physics::eCOLLISION_DOWN) {
			// Colisión en los pies
		}
		if(collisionFlags & Physics::eCOLLISION_SIDES) {
			// Colisión en los lados
		}
		if(collisionFlags & Physics::eCOLLISION_UP) {
			// Colisión con la cabeza
		}*/
	}

	//________________________________________________________________________

	Vector3 CAvatarController::estimateMotionDirection() {
		// Mediante trigonometria basica sacamos el angulo que forma el desplazamiento
		// que queremos llevar a cabo
		float displacementYaw = asin(_displacement.normalisedCopy().x);
		// Obtenemos una copia de la matriz de transformación del personaje
		Matrix4 characterTransform = _entity->getTransform();
		// Si estamos andando hacia atras, invertimos el giro
		if(_displacement.z < 0) displacementYaw *= -1;
		
		// Rotamos la matriz de transformacion tantos grados como diga el vector 
		// desplazamiento calculado de pulsar las teclas
		Math::yaw(displacementYaw, characterTransform);
		// Obtenemos el vector unitario de orientación de la matriz de transformación
		Vector3 motionDirection = Math::getDirection(characterTransform);
		// Invertimos el vector resultante si nos estamos desplazando hacia atras
		// porque el yaw se calcula de forma contraria al andar hacia atras
		if(_displacement.z < 0) motionDirection *= -1;

		// Devolvemos la dirección del movimiento estimado
		return motionDirection;
	}

	//________________________________________________________________________

	Vector3 CAvatarController::estimateGroundMotion(unsigned int msecs) {
		// Si nuestro movimiento es nulo no hacemos nada
		if(_displacement == Vector3::ZERO) return Vector3::ZERO;

		// Aplicamos la velocidad de movimiento
		//characterMovement *= _acceleration * msecs;
		return estimateMotionDirection() * _acceleration * msecs;
	}

	//________________________________________________________________________

	Vector3 CAvatarController::estimateAirMotion(unsigned int msecs) {
		return estimateGroundMotion(msecs) + _gravity;
	}

	//________________________________________________________________________

	void CAvatarController::executeMovementCommand(ControlType commandType) {
		_displacement += _movementCommands[commandType];
	}

	//________________________________________________________________________

	void CAvatarController::initMovementCommands() {
		_movementCommands[Control::WALK]				= Vector3(0,0,1);
		_movementCommands[Control::WALKBACK]			= Vector3(0,0,-1);
		_movementCommands[Control::STRAFE_LEFT]			= Vector3(1,0,0);
		_movementCommands[Control::STRAFE_RIGHT]		= Vector3(-1,0,0);
		_movementCommands[Control::STOP_WALK]			= Vector3(0,0,-1);
		_movementCommands[Control::STOP_WALKBACK]		= Vector3(0,0,1);
		_movementCommands[Control::STOP_STRAFE_LEFT]	= Vector3(-1,0,0);
		_movementCommands[Control::STOP_STRAFE_RIGHT]	= Vector3(1,0,0);
	}

} // namespace Logic

