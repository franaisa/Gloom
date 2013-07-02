/**
@file Ragdoll.h

@see Logic::CRagdoll
@see Logic::IComponent

@author Francisco Aisa García
@date Julio, 2013
*/

#include "Ragdoll.h"
#include "AnimatedGraphics.h"
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
using namespace std;

IMP_FACTORY(CRagdoll);

//________________________________________________________________________

CRagdoll::CRagdoll() {
	// Nada que hacer
}

//________________________________________________________________________

CRagdoll::~CRagdoll() {
	// Nada que hacer
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
	_animatedGraphicsComponent = _entity->getComponent<CAnimatedGraphics>("CAnimatedGraphics");
	assert(_animatedGraphicsComponent != NULL && "Error: Los componentes de ragdoll necesitan tener entidades animadas");

	return true;
} 

//________________________________________________________________________

// @deprecated Toda esta parte esta deprecada, deberiamos tener un iterador al hueso
// bindeada al collider de manera que no se hagan busquedas en el tick

void CRagdoll::onFixedTick(unsigned int msecs) {
	// Actualizar en base a las posiciones de los bones
	Vector3 position; Quaternion orientation;
	
	vector<Physics::CDynamicEntity*> boneList = _aggregate.getEntities();
	for(int i = 0; i < boneList.size(); ++i) {
		boneList[i]->getGlobalPose(position, orientation);
		_animatedGraphicsComponent->setBonePose( boneList[i]->getName(), position, orientation );
	}
}

//________________________________________________________________________

void CRagdoll::onStart() {
	// Bindeamos los huesos de la entidad gráfica a los colliders del
	// ragdoll
	Vector3 position; Quaternion orientation;

	vector<Physics::CDynamicEntity*> boneList = _aggregate.getEntities();
	for(int i = 0; i < boneList.size(); ++i) {
		_animatedGraphicsComponent->getBonePose(boneList[i]->getName(), position, orientation);

		boneList[i]->setGlobalPose(position, orientation, false);
		//boneList[i]->setKinematic(false);
	}
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
	_aggregate.load(file, group, groupList, this, true);
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

void CRagdoll::deactivateSimulation() {
	_aggregate.deactivateSimulation();
}

//________________________________________________________________________

void CRagdoll::activateSimulation() {
	_aggregate.activateSimulation();
}