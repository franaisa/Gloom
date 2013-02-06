/**
@file PhysicEntity.h

Contiene la implementación del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/SwitchTrigger.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageKinematicMove.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicEntity);

//---------------------------------------------------------

CPhysicEntity::CPhysicEntity() : IPhysics(), _actor(NULL), _movement(0,0,0)
{
	_server = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicEntity::~CPhysicEntity() 
{
	if (_actor) {
		_server->destroyActor(_actor);
		_actor = NULL;
	}

	_server = NULL;
} 

////---------------------------------------------------------

bool CPhysicEntity::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al método de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el objeto físico asociado al componente
	_actor = createActor(entityInfo);

	return true;
} 

//---------------------------------------------------------

bool CPhysicEntity::accept(CMessage *message)
{
	return message->getMessageType() == Message::KINEMATIC_MOVE;
}

//---------------------------------------------------------

void CPhysicEntity::process(CMessage *message)
{

	switch(message->getMessageType()) {
	case Message::KINEMATIC_MOVE:
		// Acumulamos el vector de desplazamiento para usarlo posteriormente en 
		// el método tick.
		_movement += ((CMessageKinematicMove*)message)->getMovement();
		break;
	}
}

//---------------------------------------------------------

void CPhysicEntity::tick(unsigned int msecs) 
{

	// Invocar al método de la clase padre (IMPORTANTE)
	IComponent::tick(msecs);

	// Si es una entidad estática no hacemos nada
	PxRigidDynamic *dinActor = _actor->isRigidDynamic();
	if (!dinActor) 
		return;

	// Actualizar la posición y la orientación de la entidad lógica usando la 
	// información proporcionada por el motor de física	
	_entity->setTransform(_server->getActorTransform(dinActor));

	// Si el objeto físico es cinemático intentamos moverlo de acuerdo 
	// a los mensajes KINEMATIC_MOVE recibidos 
	if (_server->isKinematic(dinActor)) {
		_server->moveKinematicActor(dinActor, _movement);
		_movement = Vector3::ZERO;
	} 
}

//---------------------------------------------------------

PxRigidActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
{
	// Leer el tipo de entidad
	assert(entityInfo->hasAttribute("physic_entity"));
	const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	assert((physicEntity == "rigid") || (physicEntity == "plane") || (physicEntity == "fromFile"));

	// Crear el tipo de entidad adecuada
	if (physicEntity == "plane") 
		return createPlane(entityInfo);
	
	if (physicEntity == "rigid") 
		return createRigid(entityInfo);
	
	if (physicEntity == "fromFile")
		return createFromFile(entityInfo);

	return NULL;
}

PxRigidStatic* CPhysicEntity::createPlane(const Map::CEntity *entityInfo)
{
	// La posición de la entidad es un punto del plano
	const Vector3 point = _entity->getPosition();
	
	// Leer el vector normal al plano
	assert(entityInfo->hasAttribute("physic_normal"));
	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");

	// Leer el grupo de colisión (por defecto grupo 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");
 
	// Crear el plano
	return _server->createPlane(point, normal, group, this);
}

PxRigidActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo)
{
	// Leer la posición de la entidad
	const Vector3 position = _entity->getPosition();
	
	// Leer el tipo de entidad: estáticos, dinámico o cinemático
	assert(entityInfo->hasAttribute("physic_type"));
	const std::string physicType = entityInfo->getStringAttribute("physic_type");
	assert((physicType == "static") || (physicType == "dynamic") || (physicType == "kinematic"));

	// Leer la forma (shape)
	assert(entityInfo->hasAttribute("physic_shape"));
	const std::string physicShape = entityInfo->getStringAttribute("physic_shape");
	assert(physicShape == "box");

	// Leer si es un trigger (por defecto no)
	bool trigger = false;
	if (entityInfo->hasAttribute("physic_trigger"))
		trigger = entityInfo->getBoolAttribute("physic_trigger");

	// Leer el grupo de colisión (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");


	if (physicType == "static") {
		if (physicShape == "box") {
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja estática
			return _server->createStaticBox(position, physicDimensions, trigger, group, this);
		}

	} else {
		// Leer la masa (por defecto 0)
		float mass = 0;
		if (entityInfo->hasAttribute("physic_mass"))
			mass = entityInfo->getFloatAttribute("physic_mass");
		
		// Leer si se trata de un actor cinemático
		bool kinematic = (physicType == "kinematic");

		if (physicShape == "box") {
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja dinámica
			return _server->createDynamicBox(position, physicDimensions, mass, kinematic, trigger, group, this);
		}
	}

	return NULL;
}

PxRigidActor* CPhysicEntity::createFromFile(const Map::CEntity *entityInfo)
{
	// Leer la ruta del fichero RepX
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");
	
	// Leer el grupo de colisión (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	// Crear el actor a partir del fichero RepX
	return _server->createFromFile(file, group, this);
}


void CPhysicEntity::onTrigger(IPhysics *otherComponent, bool enter)
{
	// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
	// todos los componentes de la entidad. 
	if(!_activate)
		return;

	if (enter) {
		Logic::CMessageTouched *m = new Logic::CMessageTouched();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	} else {
		Logic::CMessageUntouched *m = new Logic::CMessageUntouched();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	}
}

