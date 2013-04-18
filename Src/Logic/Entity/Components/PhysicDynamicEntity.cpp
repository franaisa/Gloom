/**
@file PhysicDynamicEntity.h

@see Logic::CPhysicDynamicEntity
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "PhysicDynamicEntity.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"
#include "Physics/MaterialManager.h"

#include "Logic/Messages/MessageKinematicMove.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageContactExit.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Logic;
using namespace Physics;

IMP_FACTORY(CPhysicDynamicEntity);

//---------------------------------------------------------

CPhysicDynamicEntity::CPhysicDynamicEntity() : _movement(0,0,0),
											   _inTrigger(false),
											   _inContact(false),
											   _sleepUntil(false) {

	_server = CServer::getSingletonPtr();
	_geometryFactory = Physics::CGeometryFactory::getSingletonPtr();
	_materialManager = Physics::CMaterialManager::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicDynamicEntity::~CPhysicDynamicEntity() {
	// El destructor de _physicEntity ya sabe que hacer

	_server = NULL;
} 

////---------------------------------------------------------

bool CPhysicDynamicEntity::spawn(Logic::CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
	// Invocar al método de la clase padre
	if( !IComponent::spawn(entity,map,entityInfo) ) return false;

	// Crear el objeto físico asociado al componente
	createPhysicEntity(entityInfo);

	return true;
} 

//---------------------------------------------------------

bool CPhysicDynamicEntity::accept(CMessage *message) {
	Logic::TMessageType msgType = message->getMessageType();

	return msgType == Message::KINEMATIC_MOVE ||
		   msgType == Message::ACTIVATE ||
		   msgType == Message::DEACTIVATE ||
		   msgType == Message::SET_PHYSIC_POSITION ||
		   msgType == Message::ADD_FORCE_PHYSICS;
}

//---------------------------------------------------------

void CPhysicDynamicEntity::process(CMessage *message) {
	switch(message->getMessageType()) {
		case Message::KINEMATIC_MOVE:
			// Acumulamos el vector de desplazamiento para usarlo posteriormente en 
			// el método tick.
			_movement += static_cast<CMessageKinematicMove*>(message)->getMovement();
			break;
		case Message::ACTIVATE:
			activateSimulation();
		
			break;
		case Message::DEACTIVATE:
		{
			deactivateSimulation();
		
			break;
		}
		case Message::SET_PHYSIC_POSITION:
		{
			CMessageSetPhysicPosition* setPosMsg = static_cast<CMessageSetPhysicPosition*>(message);
			setPosition( setPosMsg->getPosition(), setPosMsg->getMakeConversion() );
		
			break;
		}
		case Message::ADD_FORCE_PHYSICS:
		{
			CMessageAddForcePhysics* forceMsg = static_cast<CMessageAddForcePhysics*>(message);

			if( !forceMsg->getGravity() )
				_physicEntity.disableGravity(true);
		
			addForce( forceMsg->getForceVector(), forceMsg->getForceMode() );
		
			break;
		}
	}
}

//---------------------------------------------------------

void CPhysicDynamicEntity::tick(unsigned int msecs) {
	// Actualizar la posición y la orientación de la entidad lógica usando la 
	// información proporcionada por el motor de física	
	_entity->setTransform( _physicEntity.getTransform() );

	// Si el objeto físico es kinematico intentamos moverlo de acuerdo 
	// a los mensajes KINEMATIC_MOVE recibidos 
	if ( _physicEntity.isKinematic() ) {
		_physicEntity.move(_movement);
		_movement = Vector3::ZERO;
	}
}

//---------------------------------------------------------

void CPhysicDynamicEntity::readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList) {
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

//---------------------------------------------------------

void CPhysicDynamicEntity::createPhysicEntity(const Map::CEntity *entityInfo) {
	// Leemos el tipo de entidad
	assert(entityInfo->hasAttribute("physic_entity"));
	const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	assert((physicEntity == "rigid") || (physicEntity == "fromFile"));

	// Leemos la informacion de grupos de colision
	int group = 0;
	std::vector<int> groupList;
	readCollisionGroupInfo(entityInfo, group, groupList);
	
	if (physicEntity == "rigid") 
		createRigid(entityInfo, group, groupList);
	
	if (physicEntity == "fromFile")
		createFromFile(entityInfo, group, groupList);
}

//---------------------------------------------------------

void CPhysicDynamicEntity::createRigid(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList) {
	// Leer la posición de la entidad
	const Vector3 position = _entity->getPosition();
	
	// Leer el tipo de entidad: estáticos, dinámico o cinemático
	assert(entityInfo->hasAttribute("physic_type"));
	const std::string physicType = entityInfo->getStringAttribute("physic_type");
	assert((physicType == "dynamic") || (physicType == "kinematic"));

	// Leer la forma (shape)
	assert(entityInfo->hasAttribute("physic_shape"));
	const std::string physicShape = entityInfo->getStringAttribute("physic_shape");
	assert(physicShape == "box" || physicShape == "sphere");

	// Leer si es un trigger (por defecto no)
	bool isTrigger = false;
	if (entityInfo->hasAttribute("physic_trigger"))
		isTrigger = entityInfo->getBoolAttribute("physic_trigger");


	// Leer la masa (por defecto 0)
	float mass = 0;
	if (entityInfo->hasAttribute("physic_mass"))
		mass = entityInfo->getFloatAttribute("physic_mass");
		
	// Leer si se trata de un actor cinemático
	bool isKinematic = (physicType == "kinematic");

	if (physicShape == "box") {
		// Leer las dimensiones de la caja
		assert(entityInfo->hasAttribute("physic_dimensions"));
		const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");

		Physics::BoxGeometry box = _geometryFactory->createBox(physicDimensions);
		Physics::Material* defaultMaterial = _materialManager->getMaterial(eDEFAULT);
		float density = mass / (physicDimensions.x * physicDimensions.y * physicDimensions.z);
		
		_physicEntity.load(position, box, *defaultMaterial, density, isKinematic, isTrigger, group, groupList, this);
	}
	else if (physicShape == "sphere") {
		assert(entityInfo->hasAttribute("physic_radius"));
		const float physicRadius = entityInfo->getFloatAttribute("physic_radius");
		
		Physics::SphereGeometry sphere = _geometryFactory->createSphere(physicRadius);
		Physics::Material* defaultMaterial = _materialManager->getMaterial(eDEFAULT);
		float density = mass / (4.0/3.0 * Math::PI * physicRadius * physicRadius * physicRadius);

		_physicEntity.load(position, sphere, *defaultMaterial, density, isKinematic, isTrigger, group, groupList, this);
	}
}

//---------------------------------------------------------

void CPhysicDynamicEntity::createFromFile(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList) {
	// Leer la ruta del fichero RepX
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");

	// Crear el actor a partir del fichero RepX
	_physicEntity.load(file, group, groupList, this);
}

//---------------------------------------------------------

void CPhysicDynamicEntity::onTrigger(IPhysics *otherComponent, bool enter) {
	// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
	// todos los componentes de la entidad. 

	if (enter) {
		_inTrigger=true;
		Logic::CMessageTouched *m = new Logic::CMessageTouched();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	} else {
		_inTrigger=false;
		Logic::CMessageUntouched *m = new Logic::CMessageUntouched();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	}
}

//---------------------------------------------------------

void CPhysicDynamicEntity::onContact (IPhysics *otherComponent,bool enter) {
	if (enter) {
		_inContact=true;
		Logic::CMessageContactEnter* msg = new Logic::CMessageContactEnter();
		msg->setEntity( otherComponent->getEntity() );
		_entity->emitMessage(msg);
	} else {
		_inContact=false;
		Logic::CMessageContactExit *m = new Logic::CMessageContactExit();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	}
}
//---------------------------------------------------------

/*void CPhysicDynamicEntity::onShapeHit (const PxControllerShapeHit &hit) {
	//std::cout << _entity->getName() << " le llega el contacto con " << ((IPhysics*)(hit.controller->getUserData()))->getEntity()->getName() << std::endl;

}*/

//---------------------------------------------------------

void CPhysicDynamicEntity::setPosition(const Vector3 &position, bool makeConversionToLogicWorld) {
	_physicEntity.setPosition(position, makeConversionToLogicWorld);
}
//---------------------------------------------------------

void CPhysicDynamicEntity::setTransform(const Matrix4 &transform, bool makeConversionToLogicWorld) {
	_physicEntity.setTransform(transform, makeConversionToLogicWorld);
}
//---------------------------------------------------------

void CPhysicDynamicEntity::addForce(const Vector3& force, Physics::ForceMode mode, bool autowake) {
	_physicEntity.addForce(force, mode, autowake);
}

//---------------------------------------------------------

void CPhysicDynamicEntity::addTorque(const Vector3& force, Physics::ForceMode mode, bool autowake) {
	_physicEntity.addTorque(force, mode, autowake);
}

//---------------------------------------------------------

void CPhysicDynamicEntity::deactivateSimulation() {
	_physicEntity.deactivateSimulation();
}

//---------------------------------------------------------

void CPhysicDynamicEntity::activateSimulation() {
	_physicEntity.activateSimulation();
}