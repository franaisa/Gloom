/**
@file Ragdoll.h

@see Logic::CRagdoll
@see Logic::IComponent

@author Francisco Aisa García
@date Julio, 2013
*/

#include "Ragdoll.h"
#include "AnimatedGraphics.h"
#include "Map/MapEntity.h"
#include "Physics/DynamicEntity.h"

#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageContactExit.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Logic;
using namespace Physics;
using namespace std;

IMP_FACTORY(CRagdoll);

//________________________________________________________________________

CRagdoll::CRagdoll() : _ragdollHasControl(false) {
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

	return true;
}

//________________________________________________________________________

bool CRagdoll::accept(const std::shared_ptr<CMessage>& message) {
	TMessageType msgType = message->getMessageType();

	return msgType == Message::PLAYER_DEAD	||
		   msgType == Message::PLAYER_SPAWN;
} // accept

//________________________________________________________________________

void CRagdoll::process(const std::shared_ptr<CMessage>& message) {
	switch( message->getMessageType() ) {
		case Message::PLAYER_DEAD: {
			// Indicamos que la fisica tiene el control
			_ragdollHasControl = true;

			// Convertimos todas las shapes fisicas en dinamicas para que
			// el motor de fisicas tome el control
			for(int i = 0; i < _ragdollBonesBuffer.size(); ++i) {
				_ragdollBonesBuffer[i].first.setManuallyControlled(true);
				//_ragdollBonesBuffer[i].second->setKinematic(false);
			}

			break;
		}
		case Message::PLAYER_SPAWN: {
			// Indicamos que la lógica tiene el control
			_ragdollHasControl = false;

			// Convertimos todas las shapes fisicas en kinemáticas para
			// alinear los colliders a los huesos en cada frame de la animación
			for(int i = 0; i < _ragdollBonesBuffer.size(); ++i) {
				_ragdollBonesBuffer[i].first.setManuallyControlled(false);
				//_ragdollBonesBuffer[i].second->setKinematic(true);
			}

			break;
		}
	}
} // process

//________________________________________________________________________

void CRagdoll::onFixedTick(unsigned int msecs) {
	// Actualizamos las posiciones y orientaciones de los bones en base a la
	// fisica si estamos ejecutando el ragdoll o en base a las animaciones si
	// estamos haciendo de hitboxes
	Vector3 position; Quaternion orientation;
	/*if(_ragdollHasControl) {
		for(int i = 0; i < _ragdollBonesBuffer.size(); ++i) {
			_ragdollBonesBuffer[i].second->getGlobalPose(position, orientation); // Sacamos la orientacion de lo fisico
			_ragdollBonesBuffer[i].first.setGlobalPose(position, orientation); // Seteamos la orientacion a lo gráfico
		}
	}
	else {
		for(int i = 0; i < _ragdollBonesBuffer.size(); ++i) {
			_ragdollBonesBuffer[i].first.getGlobaPose(position, orientation); // Sacamos la orientacion de los huesos graficos
			_ragdollBonesBuffer[i].second->setGlobalPose(position, orientation, false); // Seteamos la orientacion de lo fisico
		}
	}
	*/
	
	
	for(int i = 0; i < _ragdollBonesBuffer.size(); ++i) {
		_ragdollBonesBuffer[i].second->getGlobalPose(position, orientation); // Sacamos la orientacion de lo fisico
		_ragdollBonesBuffer[i].first.setGlobalPose(position, orientation); // Seteamos la orientacion a lo gráfico
	}
	/* */
}

//________________________________________________________________________

void CRagdoll::onStart() {
	// Nos aseguramos de que exista un componente gráfico animado
	CAnimatedGraphics* animComp = _entity->getComponent<CAnimatedGraphics>("CAnimatedGraphics");
	assert(animComp != NULL && "Error: Los componentes de ragdoll necesitan tener entidades animadas");

	// Obtenemos la lista de huesos que tiene un collider asignado
	// Bindeamos los colliders con los respectivos huesos de la entidad gráfica
	vector<Physics::CDynamicEntity*> boneList = _aggregate.getEntities();
	unsigned int nbBones = boneList.size();
	_ragdollBonesBuffer.reserve(nbBones);
	for(int i = 0; i < nbBones; ++i) {
		boneList[i]->setKinematic(true);
		_ragdollBonesBuffer.push_back( pair<Graphics::CBone, Physics::CDynamicEntity*>( animComp->getBone( boneList[i]->getName() ), boneList[i] ) );
	}

	// Colocamos los colliders en la posicion en la que se encuentren los bones
	// de primeras
	Vector3 position; Quaternion orientation;
	for(int i = 0; i < _ragdollBonesBuffer.size(); ++i) {
		_ragdollBonesBuffer[i].first.getGlobaPose(position, orientation); // Sacamos la orientacion de los huesos graficos
		_ragdollBonesBuffer[i].second->setGlobalPose(position, orientation, false); // Seteamos la orientacion de lo fisico
		//_ragdollBonesBuffer[i].second->setKinematic(false);
	}
}

//________________________________________________________________________

void CRagdoll::onActivate() {
	_ragdollHasControl = false;
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