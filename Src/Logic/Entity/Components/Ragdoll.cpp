/**
@file Ragdoll.h

@see Logic::CRagdoll
@see Logic::IComponent

@author Francisco Aisa García
@date Julio, 2013
*/

#include "Ragdoll.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"
#include "Physics/MaterialManager.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageUntouched.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageTransform.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageContactExit.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Logic;
using namespace Physics;

IMP_FACTORY(CRagdoll);

//________________________________________________________________________

CRagdoll::CRagdoll() {
	_server = CServer::getSingletonPtr();
	_geometryFactory = Physics::CGeometryFactory::getSingletonPtr();
	_materialManager = Physics::CMaterialManager::getSingletonPtr();
}

//________________________________________________________________________

CRagdoll::~CRagdoll() {
	// El destructor de _physicEntity ya sabe que hacer
	_server = NULL;
} 

//________________________________________________________________________

bool CRagdoll::spawn(Logic::CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
	// Invocar al método de la clase padre
	if( !IComponent::spawn(entity,map,entityInfo) ) return false;

	assert( entityInfo->hasAttribute("ragdoll_file") );
	assert( entityInfo->hasAttribute("ragdoll_group") );
	assert( entityInfo->hasAttribute("ragdoll_groupList") );

	// Leemos la informacion de grupos de colision
	int group = 0;
	std::vector<int> groupList;
	readCollisionGroupInfo(entityInfo, group, groupList);

	// Creamos el ragdoll leyendo desde fichero
	loadRagdoll(entityInfo, group, groupList);

	// Nos aseguramos de que exista un componente gráfico animado
	// al que podamos bindear colliders

	return true;
} 

//________________________________________________________________________

bool CRagdoll::accept(const std::shared_ptr<CMessage>& message) {
	Logic::TMessageType msgType = message->getMessageType();

	return	msgType == Message::ACTIVATE				||
			msgType == Message::ADD_FORCE_PHYSICS;
}

//________________________________________________________________________

void CRagdoll::process(const std::shared_ptr<CMessage>& message) {
	switch( message->getMessageType() ) {
		case Message::ACTIVATE: {
			bool activate = std::static_pointer_cast<CMessageActivate>(message)->getActivated();
			if(activate)
				activateSimulation();
			else
				deactivateSimulation();

			break;
		}
		case Message::ADD_FORCE_PHYSICS: {
			std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::static_pointer_cast<CMessageAddForcePhysics>(message);

			if( !forceMsg->getGravity() )
				_physicEntity.disableGravity(true);

			addForce( forceMsg->getForceVector(), forceMsg->getForceMode() );
			break;
		}
	}
}

//________________________________________________________________________

void CRagdoll::onFixedTick(unsigned int msecs) {
	// Actualizar en base a las posiciones de los bones
}

//________________________________________________________________________

void CRagdoll::onStart() {
	// Bindeamos los huesos de la entidad gráfica a los colliders del
	// ragdoll
}

//________________________________________________________________________

void CRagdoll::readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList) {
	// Leer el grupo de colisión (por defecto grupo 0)
	group = entityInfo->getIntAttribute("ragdoll_group");

	// Comprobamos los grupos con los que esta entidad deberia colisionar
	std::istringstream groupListString( entityInfo->getStringAttribute("ragdoll_groupList") );

	// Para cada cadena entre comas...
	do {
		std::string groupNumber;
		std::getline(groupListString, groupNumber, ','); // linea entre delimitadores
				
		std::istringstream str(groupNumber);     // wrappeamos cadena como Input Stream
		do {									// Le quitamos los espacios...
			std::getline(str, groupNumber, ' ');  // linea entre espacios
		} while (groupNumber.size() == 0 && !str.eof());

		groupList.push_back( atoi(groupNumber.c_str()) );
	} while ( !groupListString.eof() );
}

//________________________________________________________________________

void CRagdoll::loadRagdoll(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList) {
	// Leer la ruta del fichero RepX
	assert( entityInfo->hasAttribute("ragdoll_file") );
	const std::string file = entityInfo->getStringAttribute("ragdoll_file");

	// Crear el actor a partir del fichero RepX
	_physicEntity.load(file, group, groupList, this, true);
}

//________________________________________________________________________

void CRagdoll::onTrigger(IPhysics *otherComponent, bool enter) {
	// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
	// todos los componentes de la entidad. 
	if (enter) {
		std::shared_ptr<CMessageTouched> m = std::make_shared<CMessageTouched>();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	} 
	else {
		std::shared_ptr<CMessageUntouched> m = std::make_shared<CMessageUntouched>();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	}
}

//________________________________________________________________________

void CRagdoll::onContact (IPhysics *otherComponent, bool enter) {
	if (enter) {
		std::shared_ptr<CMessageContactEnter> msg = std::make_shared<CMessageContactEnter>();
		msg->setEntity( otherComponent->getEntity()->getEntityID() );
		_entity->emitMessage(msg);
	} 
	else {
		std::shared_ptr<CMessageContactExit> m = std::make_shared<CMessageContactExit>();
		m->setEntity(otherComponent->getEntity()->getEntityID());
		_entity->emitMessage(m);
	}
}

//________________________________________________________________________

void CRagdoll::addForce(const Vector3& force, Physics::ForceMode mode, bool autowake) {
	_physicEntity.addForce(force, mode, autowake);
}

//________________________________________________________________________

void CRagdoll::addTorque(const Vector3& force, Physics::ForceMode mode, bool autowake) {
	_physicEntity.addTorque(force, mode, autowake);
}

//________________________________________________________________________

void CRagdoll::deactivateSimulation() {
	_physicEntity.deactivateSimulation();
}

//________________________________________________________________________

void CRagdoll::activateSimulation() {
	_physicEntity.activateSimulation();
}