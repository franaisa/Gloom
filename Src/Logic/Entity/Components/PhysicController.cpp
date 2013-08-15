/**
@file PhysicController.cpp

Contiene la implementación del componente que se utiliza para representar jugadores y enemigos en
el mundo físico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Francisco Aisa García
@date Marzo, 2013.
*/

#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageCollisionDown.h"
#include "Logic/Messages/MessageCealing.h"
#include "Logic/Messages/MessageSide.h"

#include "AvatarController.h"

using namespace Logic;
using namespace Physics;

IMP_FACTORY(CPhysicController);

//________________________________________________________________________

CPhysicController::CPhysicController() {
	// _controller <- Su valor se auto inicializa
	// en el proyecto de física
}

//________________________________________________________________________

CPhysicController::~CPhysicController() {
	// El destructor de _controller se auto ejecuta
} 

//________________________________________________________________________

bool CPhysicController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
	// Invocar al método de la clase padre
	if( !IComponent::spawn(entity,map,entityInfo) ) return false;

	// Crear el character controller asociado al componente
	createController(entityInfo);

	return true;
}

//________________________________________________________________________

void  CPhysicController::setPhysicPosition (const Vector3 &position) {
	//Teletransportamos al player y ponemos la logica en el mismo momento(sino ocurrirían teletransportaciones gráficas)
	_controller.setPosition(position);
	_entity->setPosition( _controller.getPosition() );
}
//________________________________________________________________________

const Vector3& CPhysicController::getPhysicPosition() {
	return _controller.getPosition() ;
}

//________________________________________________________________________

void CPhysicController::readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList) {
	// Leer el grupo de colisión (por defecto grupo 0)
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	// Comprobamos los grupos con los que esta entidad deberia colisionar
	if (entityInfo->hasAttribute("physic_groupList")) {
		std::istringstream groupListString(entityInfo->getStringAttribute("physic_groupList"));

		// Para cada cadena entre comas...
		do {
			std::string groupNumber;
			std::getline(groupListString, groupNumber, ','); // linea entre delimitadores
				
			std::istringstream str(groupNumber);     // wrappeamos cadena como Input Stream
			do {									// Le quitamos los espacios...
				std::getline(str, groupNumber, ' ');  // linea entre espacios
			} while (groupNumber.size() == 0 && !str.eof());

			groupList.push_back( atoi(groupNumber.c_str()) );
		} while (!groupListString.eof());
	}
}

//________________________________________________________________________

void CPhysicController::createController(const Map::CEntity *entityInfo) {
	// Obtenemos la posición de la entidad. Inicialmente colocaremos el controller
	// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
	// lo atraviesa en el primer ciclo de simulación.
	Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);
	
	// Leer el volumen de colisión del controller. Por ahora sólo admitimos cápsulas.
	std::string shape = "capsule";
	if( entityInfo->hasAttribute("physic_shape") ) {
		shape = entityInfo->getStringAttribute("physic_shape");
		assert(shape == "capsule");
	}

	// Leer el radio de la cápsula
	assert(entityInfo->hasAttribute("physic_radius"));
	float radius = entityInfo->getFloatAttribute("physic_radius");

	// Leer la altura de la cápsula
	assert(entityInfo->hasAttribute("physic_height"));
	float height = entityInfo->getFloatAttribute("physic_height");

	// Leer la informacion sobre el grupo de colision de la capsula
	int group = 0;
	std::vector<int> groupList;
	readCollisionGroupInfo(entityInfo, group, groupList);

	// Inicializar el controller de tipo capsula
	_controller.load(position, radius, height, group, groupList, this);
} 

//________________________________________________________________________

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter) {
	// Mediante patron observador
	for(auto it = _observers.begin(); it != _observers.end(); ++it) {
		(*it)->onTrigger(otherComponent, enter);
	}
}

//________________________________________________________________________

void CPhysicController::onShapeHit(IPhysics* otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) {
	// Mediante patron observador
	for(auto it = _observers.begin(); it != _observers.end(); ++it) {
		(*it)->onShapeHit(otherComponent, colisionPos, colisionNormal);
	}
}

//________________________________________________________________________

void CPhysicController::onContact (IPhysics *otherComponent, bool enter) {
	// Mediante patron observador
	for(auto it = _observers.begin(); it != _observers.end(); ++it) {
		(*it)->onContact(otherComponent, enter);
	}
}

//________________________________________________________________________

void CPhysicController::deactivateSimulation() {
	_controller.deactivateSimulation();
}

//________________________________________________________________________

void CPhysicController::activateSimulation() {
	_controller.activateSimulation();
}

//________________________________________________________________________

unsigned CPhysicController::move(const Vector3& movement, unsigned int msecs) {
	// Intentamos mover el controller a la posición que se haya calculado
	// desde avatarController
	unsigned collisionFlags = _controller.move(movement, msecs);
	
	// Actualizar la posición y orientación de la entidad lógica usando la 
	// información proporcionada por el motor de física	
	_entity->setPosition( _controller.getPosition() );

	// Devolvemos los flags de colision
	return collisionFlags;
}

//________________________________________________________________________

unsigned CPhysicController::move(const Vector3& movement, unsigned int customFilterMask, unsigned int msecs) {
	// Intentamos mover el controller a la posición que se haya calculado
	// desde avatarController aplicando un filtro de colisiones particular
	unsigned collisionFlags = _controller.move(movement, customFilterMask, msecs);

	// Actualizar la posición y orientación de la entidad lógica usando la 
	// información proporcionada por el motor de física	
	_entity->setPosition( _controller.getPosition() );

	// Devolvemos los flags de colision
	return collisionFlags;
}

//________________________________________________________________________

unsigned int CPhysicController::getDefaultFilterMask() {
	return _controller.getFilterMask();
}

//________________________________________________________________________

float CPhysicController::getCapsuleRadius() {
	return _controller.getCapsuleRadius();
}

//________________________________________________________________________

float CPhysicController::getCapsuleHeight() {
	return _controller.getCapsuleHeight();
}