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
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageKinematicMove.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageContactExit.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicEntity);

//---------------------------------------------------------

CPhysicEntity::CPhysicEntity() : IPhysics(), _actor(NULL), _movement(0,0,0), _isTrigger(false)
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

bool CPhysicEntity::spawn(Logic::CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al método de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	if(entityInfo->hasAttribute("physic_trigger")) {
		_isTrigger = entityInfo->getBoolAttribute("physic_trigger");
	}

	// Crear el objeto físico asociado al componente
	_actor = createActor(entityInfo);

	_inTrigger=false;
	_inContact=false;
	_sleepUntil=false;
	return true;
} 

//---------------------------------------------------------

bool CPhysicEntity::accept(CMessage *message)
{
	return message->getMessageType() == Message::KINEMATIC_MOVE ||
		   message->getMessageType() == Message::ACTIVATE ||
		   message->getMessageType() == Message::DEACTIVATE ||
		   message->getMessageType() == Message::SET_PHYSIC_POSITION ||
		   message->getMessageType() == Message::ADD_FORCE_PHYSICS;
}

//---------------------------------------------------------

void CPhysicEntity::process(CMessage *message)
{
	unsigned int nbShapes;
	PxShape** actorShapes;
	switch(message->getMessageType()) {
	case Message::KINEMATIC_MOVE:
		// Acumulamos el vector de desplazamiento para usarlo posteriormente en 
		// el método tick.
		_movement += ((CMessageKinematicMove*)message)->getMovement();
		break;
	case Message::ACTIVATE:
		activateSimulation();
		break;
	case Message::DEACTIVATE:
		deactivateSimulation();
		break;
	case Message::SET_PHYSIC_POSITION:
		{
		CMessageSetPhysicPosition* setPosMsg = static_cast<CMessageSetPhysicPosition*>(message);
		setPhysicPosition( setPosMsg->getPosition(), setPosMsg->getMakeConversion() );
		break;
		}
	case Message::ADD_FORCE_PHYSICS:
		if(!((CMessageAddForcePhysics*)message)->getGravity())
			_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		addImpulsiveForce( ((CMessageAddForcePhysics*)message)->getForceVector() );
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

void CPhysicEntity::readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList) {
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

PxRigidActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
{
	// Leer el tipo de entidad
	assert(entityInfo->hasAttribute("physic_entity"));
	const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	assert((physicEntity == "rigid") || (physicEntity == "plane") || (physicEntity == "fromFile"));

	int group = 0;
	std::vector<int> groupList;
	readCollisionGroupInfo(entityInfo, group, groupList);

	// Crear el tipo de entidad adecuada
	if (physicEntity == "plane") 
		return createPlane(entityInfo, group, groupList);
	
	if (physicEntity == "rigid") 
		return createRigid(entityInfo, group, groupList);
	
	if (physicEntity == "fromFile")
		return createFromFile(entityInfo, group, groupList);

	return NULL;
}

//---------------------------------------------------------

PxRigidStatic* CPhysicEntity::createPlane(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList)
{
	// La posición de la entidad es un punto del plano
	const Vector3 point = _entity->getPosition();
	
	// Leer el vector normal al plano
	assert(entityInfo->hasAttribute("physic_normal"));
	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");
 
	// Crear el plano
	//return _server->createPlane(point, normal, group, this);
	return _server->createPlane(point, normal, group, groupList, this);
}

//---------------------------------------------------------

PxRigidActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList)
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
	assert(physicShape == "box" || physicShape == "sphere");

	// Leer si es un trigger (por defecto no)
	bool trigger = false;
	if (entityInfo->hasAttribute("physic_trigger"))
		trigger = entityInfo->getBoolAttribute("physic_trigger");

	if (physicType == "static") {
		if (physicShape == "box") {
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja estática
			return _server->createStaticBox(position, physicDimensions, trigger, group, groupList, this);
		}
		else if (physicShape == "sphere") {
			assert(entityInfo->hasAttribute("physic_radius"));
			const float physicRadius = entityInfo->getFloatAttribute("physic_radius");
			
			// Crear una esfera dinámica
			return _server->createStaticSphere(position, physicRadius, trigger, group, groupList, this);
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
			return _server->createDynamicBox(position, physicDimensions, mass, kinematic, trigger, group, groupList, this);
		}
		else if (physicShape == "sphere") {
			assert(entityInfo->hasAttribute("physic_radius"));
			const float physicRadius = entityInfo->getFloatAttribute("physic_radius");
			
			// Crear una esfera dinámica
			return _server->createDynamicSphere(position, physicRadius, mass, kinematic, trigger, group, groupList, this);
		}
	}

	return NULL;
}

//---------------------------------------------------------

PxRigidActor* CPhysicEntity::createFromFile(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList)
{
	// Leer la ruta del fichero RepX
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");

	// Crear el actor a partir del fichero RepX
	return _server->createFromFile(file, group, groupList, this);
}

//---------------------------------------------------------

void CPhysicEntity::onTrigger(IPhysics *otherComponent, bool enter)
{
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

void CPhysicEntity::onContact (IPhysics *otherComponent,bool enter) {
	std::cout << "contactan con la entidad " << _entity->getName() << std::endl;
	if (enter) {
		_inContact=true;
		std::cout << "TOCO LA ENTIDAD" << std::endl;
			Logic::CMessageContactEnter* msg = new Logic::CMessageContactEnter();
			msg->setEntity( otherComponent->getEntity() );
			_entity->emitMessage(msg);
	} else {
		_inContact=false;
		std::cout << "DEJO DE TOCAR LA ENTIDAD" << std::endl;
		Logic::CMessageContactExit *m = new Logic::CMessageContactExit();
		m->setEntity(otherComponent->getEntity());
		_entity->emitMessage(m);
	}
}
//---------------------------------------------------------


void CPhysicEntity::onShapeHit (const PxControllerShapeHit &hit)
{
	//std::cout << _entity->getName() << " le llega el contacto con " << ((IPhysics*)(hit.controller->getUserData()))->getEntity()->getName() << std::endl;

}

//---------------------------------------------------------

void CPhysicEntity::setPhysicPosition(const Vector3 &position, bool makeConversionToLogicWorld) {
	if( _actor->isRigidBody() )
		_server->setRigidBodyPosition( static_cast<PxRigidBody*>(_actor), position, makeConversionToLogicWorld );
}

//---------------------------------------------------------

void CPhysicEntity::addImpulsiveForce(const Vector3& force) {
	if( _actor->isRigidDynamic() ) {
		_server->addImpulsiveForce( static_cast<PxRigidDynamic*>(_actor), force ); 
	}
}


//---------------------------------------------------------

void CPhysicEntity::deactivateSimulation() {
	// Desactivamos todos los shapes del componente por completo en PhysX
	// Para ello, obtenemos todos sus shapes y ponemos los flags a false

	int nbShapes = _actor->getNbShapes();
	PxShape** actorShapes = new PxShape* [nbShapes];
	_actor->getShapes(actorShapes, nbShapes);
	for(int i = 0; i < nbShapes; ++i) {
		if(_isTrigger) {
			// Desactivamos la shape como trigger
			actorShapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
		}

		// Esta shape no tomara parte en barridos, raycasts...
		actorShapes[i]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
		// Esta shape no entrara dentro de la simulacion de fisicas
		actorShapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE , false);
	}

	delete [] actorShapes;
}

//---------------------------------------------------------

void CPhysicEntity::activateSimulation() {
	// Activamos todos los shapes del componente por completo en PhysX
	// Para ello, obtenemos todos sus shapes y ponemos los flags a true

	int nbShapes = _actor->getNbShapes();
	PxShape** actorShapes = new PxShape* [nbShapes];
	_actor->getShapes(actorShapes, nbShapes);
	for(int i = 0; i < nbShapes; ++i) {
		if(_isTrigger) {
			// Volvemos a activar la shape como trigger
			actorShapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		// Esta shape tomara parte en barridos, raycasts...
		actorShapes[i]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		// Esta shape entrara dentro de la simulacion de fisicas
		actorShapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE , true);
	}

	delete [] actorShapes;
}