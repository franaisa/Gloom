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
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Components/Camera.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"

namespace Logic {

	IMP_FACTORY(CSpectatorController);

	//________________________________________________________________________

	CSpectatorController::CSpectatorController() : _frictionCoef(0.95f),
												   _acceleration(0.018f), 
												   _maxVelocity(2.0f) {

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
					rotationXY( mouseMsg->getMouse() );
				}

				break;
			}
		}

	} // process	

	//________________________________________________________________________

	void CSpectatorController::rotationXY(float XYturn[]) {
		//Aplicamos las rotaciones pertinentes
		 Ogre::Real pitchAngle;
		 Ogre::Real pitchAngleSign;
 
		 //Rotamos el Yaw de la entidad de acuerdo a los grados en radianes pasados como parámetro.
		 _entity->rotate(Orientation::eYAW,Ogre::Radian(XYturn[0]));
 
		 //Rotamos el Pitch de la entidad de acuerdo a los grados en radianes pasados como parámetro.
		 _entity->rotate(Orientation::ePITCH,Ogre::Radian(XYturn[1]));
		
		 // Ángulo de rotación sobre el eje X.
		 pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(_entity->getPitch().w)).valueDegrees());
 
		 // Para saber el sentido.
		 pitchAngleSign = _entity->getPitch().x;
 
		 // Limitamos el angulo un poco menos de +90/-90 para evitar perder la direccion (Tomando como referencia a Quake3)
		 if (pitchAngle > 88.0f)
		 {
			 if (pitchAngleSign > 0)
				 //Fijando a +88. Formo el quaternion previamente calculado, ahorrando procesamiento
				 _entity->setPitch(Ogre::Quaternion(0.71934,0.694658, 0, 0),false);
			 else if (pitchAngleSign < 0)
				 //Fijando a -88. Formo el quaternion previamente calculado, ahorrando procesamiento
				 _entity->setPitch(Ogre::Quaternion(0.71934,-0.694658, 0, 0),false);
		 }
		
		 //Actualizamos la orientacion(en un futuro no estara, esta por el transform que no se ha quitado)
		 _entity->setOrientation(_entity->getQuatOrientation());
	
	}//rotateXY
	//________________________________________________________________________

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

	Vector3 CSpectatorController::estimateMotionDirection() const {

		// Si nuestro movimiento es nulo no hacemos nada
		if(_displacementDir * Vector3(1, 0, 1) == Vector3::ZERO) return Vector3::ZERO;

		// Mediante trigonometria basica sacamos el angulo que forma el desplazamiento
		// que queremos llevar a cabo
		float displacementYaw = asin(_displacementDir.normalisedCopy().x);
		// Obtenemos una copia de la matriz de transformación del personaje
		Matrix4 characterTransform;
		characterTransform.makeTransform(_entity->getPosition(),Vector3::UNIT_SCALE,_entity->getQuatOrientation());
		// Si estamos andando hacia atras, invertimos el giro
		if(_displacementDir.z < 0) displacementYaw *= -1;

		// Rotamos la matriz de transformacion tantos grados como diga el vector 
		// desplazamiento calculado de pulsar las teclas
		Math::yaw(displacementYaw, characterTransform);
		// Obtenemos el vector unitario de orientación de la matriz de transformación
		Vector3 motionDirection = Math::getDirection(characterTransform);
		// Invertimos el vector resultante si nos estamos desplazando hacia atras
		// porque el yaw se calcula de forma contraria al andar hacia atras
		if(_displacementDir.z < 0) motionDirection *= -1;

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
		if(_displacementDir.z == 0) {
			_momentum += Vector3(1, 0, 1) * estimateMotionDirection() * _acceleration * msecs * msecs * 0.5f;

			if(_displacementDir.y != 0)
				_momentum += Vector3(0, 1, 0) * _displacementDir * _acceleration * msecs * msecs * 0.5f;
		}
		else {
			if(_displacementDir.y == 0) {
				_momentum += estimateMotionDirection() * _acceleration * msecs * msecs * 0.5f;
			}
			else {
				_momentum += Vector3(1, 0, 1) * estimateMotionDirection() * _acceleration * msecs * msecs * 0.5f;
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
		_movementCommands[Control::WALK]				= Vector3(0,0,1);
		_movementCommands[Control::WALKBACK]			= Vector3(0,0,-1);
		_movementCommands[Control::STRAFE_LEFT]			= Vector3(1,0,0);
		_movementCommands[Control::STRAFE_RIGHT]		= Vector3(-1,0,0);
		_movementCommands[Control::STOP_WALK]			= Vector3(0,0,-1);
		_movementCommands[Control::STOP_WALKBACK]		= Vector3(0,0,1);
		_movementCommands[Control::STOP_STRAFE_LEFT]	= Vector3(-1,0,0);
		_movementCommands[Control::STOP_STRAFE_RIGHT]	= Vector3(1,0,0);
		_movementCommands[Control::CROUCH]				= Vector3(0, -1, 0);
		_movementCommands[Control::STOP_CROUCH]			= Vector3(0, 1, 0);
		_movementCommands[Control::JUMP]				= Vector3(0, 1, 0);
		_movementCommands[Control::STOP_JUMP]			= Vector3(0, -1, 0);
		_movementCommands[Control::SIDEJUMP_LEFT]		= Vector3(1, 0, 0);
		_movementCommands[Control::SIDEJUMP_RIGHT]		= Vector3(-1, 0, 0);
		_movementCommands[Control::DODGE_BACKWARDS]		= Vector3(0, 0, -1);
		_movementCommands[Control::DODGE_FORWARD]		= Vector3(0, 0, 1);
	}

} // namespace Logic