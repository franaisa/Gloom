/**
@file SpectatorController.cpp

Contiene la implementación del componente que controla el movimiento 
del espectador.
 
@see Logic::CSpectatorController
@see Logic::IComponent

@author Francisco Aisa García
@date Abril, 2013
*/

#include "SpectatorController.h"
#include "PhysicController.h"
#include "Camera.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include "Map/MapEntity.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"

// NO DEBERIAN ESTAR AQUI!
#include "Graphics/Camera.h"
#include "Graphics/Scene.h"

namespace Logic {

	IMP_FACTORY(CSpectatorController);

	//________________________________________________________________________

	CSpectatorController::CSpectatorController() : _frictionCoef(0.95f),
												   _acceleration(0.0006f), 
												   _maxVelocity(1.2f) {

		// Inicializamos el array que contiene los vectores
		// de cada tecla de movimiento
		initMovementCommands();
	}

	//________________________________________________________________________

	CSpectatorController::~CSpectatorController() {
		// Nada que hacer
	}

	//________________________________________________________________________

	bool CSpectatorController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		/*assert( entityInfo->hasAttribute("frictionCoef") && "Error: No se ha definido el atributo frictionCoef en el mapa" );
		_frictionCoef = entityInfo->getFloatAttribute("frictionCoef");

		assert( entityInfo->hasAttribute("acceleration") && "Error: No se ha definido el atributo acceleration en el mapa" );
		_acceleration = entityInfo->getFloatAttribute("acceleration");

		assert( entityInfo->hasAttribute("maxVelocity") && "Error: No se ha definido el atributo maxVelocity en el mapa" );
		_maxVelocity = entityInfo->getFloatAttribute("maxVelocity");*/

		return true;
	} // spawn

	//________________________________________________________________________

	void CSpectatorController::onActivate() {
		_displacementDir = _momentum = Vector3::ZERO;
	} // activate

	//________________________________________________________________________

	bool CSpectatorController::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::CONTROL;
	} // accept

	//________________________________________________________________________

	void CSpectatorController::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				ControlType commandType = std::static_pointer_cast<CMessageControl>(message)->getType();

				// Comando de movimiento
				if(commandType >=0 && commandType < 18) {
					executeMovementCommand(commandType);
				}
				// Comando de raton
				else if(commandType == Control::MOUSE) {
					std::shared_ptr<CMessageMouse> mouseMsg = std::static_pointer_cast<CMessageMouse>(message);
					mouse( mouseMsg->getMouse() );
				}

				break;
			}
		}

	} // process	

	//________________________________________________________________________

	void CSpectatorController::mouse(float XYturn[]) {
		// @deprecated
		// El espectador ha dejado de funcionarme... meto la ñapa que había
		// en el avatarController para que funcione
		Graphics::CCamera* camera= _entity->getMap()->getScene()->getCamera();
		camera->moveCamera( Ogre::Radian(XYturn[0]), Ogre::Radian(XYturn[1]) );

		_entity->setOrientation( camera->getRealOrientation() );
	} // turn

	//________________________________________________________________________

	void CSpectatorController::onStart() {
		// Nos quedamos con el puntero al componente CPhysicController
		// que es el que realmente se encargará de desplazar al controlador
		// del jugador.
		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");
		assert(_physicController && "Error: El player no tiene un controlador fisico");
	}

	//________________________________________________________________________

	void CSpectatorController::onFixedTick(unsigned int msecs) {
		// Calculamos el vector de desplazamiento
		estimateMotion(msecs);

		// Movemos la cápsula en la dirección estimada por el vector de
		// desplazamiento y acumulada en el vector momentum
		_physicController->move(_momentum, msecs);
	} // tick

	//________________________________________________________________________

	Vector3 CSpectatorController::estimateMotionDirection(const Vector3& displ) const {
		// Si nuestro movimiento es nulo no hacemos nada
		if(displ * Vector3(1, 0, 1) == Vector3::ZERO) return Vector3::ZERO;

		// Mediante trigonometria basica sacamos el angulo que forma el desplazamiento
		// que queremos llevar a cabo
		float displacementYaw = asin(displ.normalisedCopy().x);
		// Obtenemos una copia de la matriz de transformación del personaje
		Matrix4 characterTransform = _entity->getTransform();
		// Si estamos andando hacia atras, invertimos el giro
		if(displ.z < 0) displacementYaw *= -1;

		// Rotamos la matriz de transformacion tantos grados como diga el vector 
		// desplazamiento calculado de pulsar las teclas
		Math::yaw(displacementYaw, characterTransform);
		// Obtenemos el vector unitario de orientación de la matriz de transformación
		Vector3 motionDirection = Math::getDirection(characterTransform);
		// Invertimos el vector resultante si nos estamos desplazando hacia atras
		// porque el yaw se calcula de forma contraria al andar hacia atras
		if(displ.z < 0) motionDirection *= -1;

		// Devolvemos la dirección del movimiento estimado
		return motionDirection;
	}

	//________________________________________________________________________

	void CSpectatorController::estimateMotion(unsigned int msecs) {
		// Aplicamos un coeficiente de rozamiento constante
		// OJO! Al no tener en cuenta los msecs, los clientes que tengan
		// un mayor frame rate se desplazaran de forma distinta
		_momentum *= _frictionCoef;

		// s = u · t + 1/2 a · t^2 <- Formula del desplazamiento
		Vector3 horizontal = Vector3(1.0f, 0.0f, 1.0f);
		if(_displacementDir.z == 0) {
			_momentum += horizontal * estimateMotionDirection( horizontal * _displacementDir ) * _acceleration * msecs * msecs * 0.5f;

			if(_displacementDir.y != 0)
				_momentum += Vector3(0, 1, 0) * _displacementDir * _acceleration * msecs * msecs * 0.5f;
		}
		else {
			if(_displacementDir.y == 0) {
				_momentum += estimateMotionDirection(_displacementDir) * _acceleration * msecs * msecs * 0.5f;
			}
			else {
				_momentum += horizontal * estimateMotionDirection( horizontal * _displacementDir ) * _acceleration * msecs * msecs * 0.5f;
				_momentum += Vector3(0, 1, 0) * _displacementDir * _acceleration * msecs * msecs * 0.5f;
			}
		}

		// Seteamos la velocidad máxima a la que se puede ir
		normalizeDirection();
	}

	//________________________________________________________________________

	void CSpectatorController::normalizeDirection() {
		// Normalizamos la velocidad al máximo
		float momVelocity = _momentum.length();
		if(momVelocity >_maxVelocity) {
			double coef = _maxVelocity / momVelocity;
			_momentum *= coef;
			
		}
	}

	//________________________________________________________________________

	void CSpectatorController::executeMovementCommand(ControlType commandType) {
		_displacementDir += _movementCommands[commandType];
	}

	//________________________________________________________________________

	void CSpectatorController::initMovementCommands() {
		_movementCommands[Control::WALK]				= Vector3(0, 0, 1);
		_movementCommands[Control::WALKBACK]			= Vector3(0, 0, -1);
		_movementCommands[Control::STRAFE_LEFT]			= Vector3(1, 0, 0);
		_movementCommands[Control::STRAFE_RIGHT]		= Vector3(-1, 0, 0);
		_movementCommands[Control::STOP_WALK]			= Vector3(0, 0, -1);
		_movementCommands[Control::STOP_WALKBACK]		= Vector3(0, 0, 1);
		_movementCommands[Control::STOP_STRAFE_LEFT]	= Vector3(-1, 0, 0);
		_movementCommands[Control::STOP_STRAFE_RIGHT]	= Vector3(1, 0, 0);
		_movementCommands[Control::CROUCH]				= Vector3(0, -1, 0);
		_movementCommands[Control::STOP_CROUCH]			= Vector3(0, 1, 0);
		_movementCommands[Control::JUMP]				= Vector3(0, 1, 0);
		_movementCommands[Control::STOP_JUMP]			= Vector3(0, -1, 0);
	}

} // namespace Logic