/**
@file PhysicStaticEntity.cpp

@see Logic::PhysicStaticEntity
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "PhysicStaticEntity.h"
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

IMP_FACTORY(CPhysicStaticEntity);

//---------------------------------------------------------

CPhysicStaticEntity::CPhysicStaticEntity() : _movement(0,0,0),
											 _inTrigger(false),
											 _inContact(false),
											 _sleepUntil(false) {

	_server = CServer::getSingletonPtr();
	_geometryFactory = Physics::CGeometryFactory::getSingletonPtr();
	_materialManager = Physics::CMaterialManager::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicStaticEntity::~CPhysicStaticEntity() {
	// El destructor de _physicEntity ya sabe que hacer

	_server = NULL;
} 

////---------------------------------------------------------

bool CPhysicStaticEntity::spawn(Logic::CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al método de la clase padre
	if( !IComponent::spawn(entity,map,entityInfo) ) return false;

	// Crear el objeto físico asociado al componente
	createPhysicEntity(entityInfo);

	return true;
} 

//---------------------------------------------------------

bool CPhysicStaticEntity::accept(const std::shared_ptr<CMessage>& message) {
	Logic::TMessageType msgType = message->getMessageType();

	return msgType == Message::ACTIVATE ||
		   msgType == Message::DEACTIVATE;
}

//---------------------------------------------------------

void CPhysicStaticEntity::process(const std::shared_ptr<CMessage>& message) {
	switch( message->getMessageType() ) {
		case Message::ACTIVATE:
			activateSimulation();
			break;

		case Message::DEACTIVATE:
			deactivateSimulation();
			break;
	}
}

//---------------------------------------------------------

void CPhysicStaticEntity::readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList) {
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

void CPhysicStaticEntity::createPhysicEntity(const Map::CEntity *entityInfo) {
	// Leer el tipo de entidad
	assert(entityInfo->hasAttribute("physic_entity"));
	const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	assert((physicEntity == "rigid") || (physicEntity == "plane") || (physicEntity == "fromFile"));

	// Leemos la informacion de grupos de colision
	int group = 0;
	std::vector<int> groupList;
	readCollisionGroupInfo(entityInfo, group, groupList);

	// Crear el tipo de entidad adecuada
	if (physicEntity == "plane") 
		createPlane(entityInfo, group, groupList);
	
	if (physicEntity == "rigid") 
		createRigid(entityInfo, group, groupList);
	
	if (physicEntity == "fromFile")
		createFromFile(entityInfo, group, groupList);

}

//---------------------------------------------------------

void CPhysicStaticEntity::createPlane(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList) {
	// La posición de la entidad es un punto del plano
	const Vector3 point = _entity->getPosition();
	
	// Leer el vector normal al plano
	assert(entityInfo->hasAttribute("physic_normal"));
	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");
 
	Physics::PlaneGeometry plane = _geometryFactory->createPlane(point, normal);
	Physics::Material* defaultMaterial = _materialManager->getMaterial(eDEFAULT);

	_physicEntity.load(plane, *defaultMaterial, group, groupList, this);
}

//---------------------------------------------------------

void CPhysicStaticEntity::createRigid(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList) {
	// Leer la posición de la entidad
	const Matrix4 transform = _entity->getTransform();
	
	// Leer el tipo de entidad: estáticos, dinámico o cinemático
	assert(entityInfo->hasAttribute("physic_type"));
	const std::string physicType = entityInfo->getStringAttribute("physic_type");
	assert(physicType == "static");

	// Leer la forma (shape)
	assert(entityInfo->hasAttribute("physic_shape"));
	const std::string physicShape = entityInfo->getStringAttribute("physic_shape");
	assert(physicShape == "box" || physicShape == "sphere");

	// Leer si es un trigger (por defecto no)
	bool isTrigger = false;
	if (entityInfo->hasAttribute("physic_trigger"))
		isTrigger = entityInfo->getBoolAttribute("physic_trigger");

	if (physicShape == "box") {
		// Leer las dimensiones de la caja
		assert(entityInfo->hasAttribute("physic_dimensions"));
		const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");

		Physics::BoxGeometry box = _geometryFactory->createBox(physicDimensions);
		Physics::Material* defaultMaterial = _materialManager->getMaterial(eDEFAULT);
		
		_physicEntity.load(transform, box, *defaultMaterial, isTrigger, group, groupList, this);
	}
	else if (physicShape == "sphere") {
		assert(entityInfo->hasAttribute("physic_radius"));
		const float physicRadius = entityInfo->getFloatAttribute("physic_radius");
		
		Physics::SphereGeometry sphere = _geometryFactory->createSphere(physicRadius);
		Physics::Material* defaultMaterial = _materialManager->getMaterial(eDEFAULT);

		_physicEntity.load(transform, sphere, *defaultMaterial, isTrigger, group, groupList, this);
	}
}

//---------------------------------------------------------

void CPhysicStaticEntity::createFromFile(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList) {
	// Leer la ruta del fichero RepX
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");

	// Crear el actor a partir del fichero RepX
	_physicEntity.load(file, group, groupList, this);
}

//---------------------------------------------------------

void CPhysicStaticEntity::onTrigger(IPhysics *otherComponent, bool enter) {
	// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
	// todos los componentes de la entidad. 

	if (enter) {
		_inTrigger=true;
		std::shared_ptr<CMessageTouched> m = std::make_shared<CMessageTouched>();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	} else {
		_inTrigger=false;
		std::shared_ptr<CMessageUntouched> m = std::make_shared<CMessageUntouched>();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	}
}

//---------------------------------------------------------

void CPhysicStaticEntity::onContact (IPhysics *otherComponent,bool enter) {
	if (enter) {
		_inContact=true;
			std::shared_ptr<CMessageContactEnter> msg = std::make_shared<CMessageContactEnter>();
			msg->setEntity( otherComponent->getEntity()->getEntityID() );
			_entity->emitMessage(msg);
	} else {
		_inContact=false;
		std::shared_ptr<CMessageContactExit> m = std::make_shared<CMessageContactExit>();
		m->setEntity(otherComponent->getEntity()->getEntityID());
		_entity->emitMessage(m);
	}
}

//---------------------------------------------------------

/*void CPhysicStaticEntity::onShapeHit (const PxControllerShapeHit &hit) {
	//std::cout << _entity->getName() << " le llega el contacto con " << ((IPhysics*)(hit.controller->getUserData()))->getEntity()->getName() << std::endl;

}*/

//---------------------------------------------------------

void CPhysicStaticEntity::deactivateSimulation() {
	_physicEntity.deactivateSimulation();
}

//---------------------------------------------------------

void CPhysicStaticEntity::activateSimulation() {
	_physicEntity.activateSimulation();
}