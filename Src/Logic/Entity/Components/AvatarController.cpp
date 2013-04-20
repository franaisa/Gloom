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
#include "Logic/Messages/MessageControl.h"
#include "Logic/Entity/Components/PhysicController.h"

namespace Logic {
	IMP_FACTORY(CAvatarController);

	CAvatarController::CAvatarController() {
		// Inicializamos el array que contiene los vectores
		// de cada tecla de movimiento
		initWalkCommands();
	}
	
	//________________________________________________________________________

	bool CAvatarController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;
		
		assert( entityInfo->hasAttribute("acceleration") && "Error: No se ha definido el atributo acceleration en el mapa" );
		_acceleration = entityInfo->getFloatAttribute("acceleration");

		return true;

	} // spawn
	
	//________________________________________________________________________

	void CAvatarController::activate() {
		IComponent::activate(); // Necesario para el onStart
		_displacement = Vector3::ZERO;
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
				CMessageControl* controlMsg = static_cast<CMessageControl*>(message);
				executeControlCommand(controlMsg);
				break;
			}
		}

	} // process
	
	//________________________________________________________________________

	void CAvatarController::mouse(const float* amount) {
		_entity->yaw(amount[0]);
		_entity->pitch(amount[1]);
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

		Vector3 direction;
		//si no nos estamos moviendo, no ejecutamos la parte del movimiento con teclas
		if(_displacement == Vector3::ZERO) {
			float yaw = asin(_displacement.normalisedCopy().x);
			Matrix4 dir = _entity->getTransform();
			Math::yaw(yaw, dir);
			

			if(_displacement.z!=0)
				direction = (Math::getDirection(dir)).normalisedCopy() * _displacement.z;
			else
				direction = (Math::getDirection(dir)).normalisedCopy();
		}






		//direction *= _acceleration * msecs;
		direction *= _acceleration;

		manageCollisions( _physicController->moveController(direction, msecs) );

		_entity->setPosition( _entity->getPosition() + direction );

	} // tick

	//________________________________________________________________________

	void CAvatarController::executeControlCommand(CMessageControl *controlMsg) {
		ControlType commandType = controlMsg->getType();

		if(commandType >= 0 && commandType < 8) {
			_displacement += _walkCommands[commandType];
		}
	}

	//________________________________________________________________________

	void CAvatarController::manageCollisions(unsigned flags) {
		// Cambiar el estado de la clase en funcion de los flags de colision
	}

	//________________________________________________________________________

	void CAvatarController::initWalkCommands() {
		_walkCommands[Control::WALK]				= Vector3(0,0,1);
		_walkCommands[Control::WALKBACK]			= Vector3(0,0,-1);
		_walkCommands[Control::STRAFE_LEFT]			= Vector3(1,0,0);
		_walkCommands[Control::STRAFE_RIGHT]		= Vector3(-1,0,0);
		_walkCommands[Control::STOP_WALK]			= Vector3(0,0,-1);
		_walkCommands[Control::STOP_WALKBACK]		= Vector3(0,0,1);
		_walkCommands[Control::STOP_STRAFE_LEFT]	= Vector3(-1,0,0);
		_walkCommands[Control::STOP_STRAFE_RIGHT]	= Vector3(1,0,0);
	}

} // namespace Logic

