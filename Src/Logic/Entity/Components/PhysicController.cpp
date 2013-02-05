/**
@file PhysicController.cpp

Contiene la implementación del componente que se utiliza para representar jugadores y enemigos en
el mundo físico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageAvatarWalk.h"
#include "Logic/Messages/MessageCollisionDown.h"

#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicController);

//---------------------------------------------------------

CPhysicController::CPhysicController() : IPhysics(), _controller(NULL), 
								       _movement(0,0,0), _falling(false)
{
	_server = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicController::~CPhysicController() 
{
	if (_controller) {
		_controller->release();
		_controller = NULL;
	}

	_server = NULL;
} 

//---------------------------------------------------------

bool CPhysicController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
{
	// Invocar al método de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el character controller asociado al componente
	_controller = createController(entityInfo);

	return true;
}

//---------------------------------------------------------

bool CPhysicController::accept(CMessage *message)
{
	return message->getMessageType() == Message::AVATAR_WALK;
} 

//---------------------------------------------------------

void CPhysicController::process(CMessage *message)
{
	switch(message->getMessageType())
	{
	case Message::AVATAR_WALK:
		// Anotamos el vector de desplazamiento para usarlo posteriormente en 
		// el método tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
		// en el mismo ciclo sólo tendremos en cuenta el último.
		_movement = ((CMessageAvatarWalk*)message)->getDirection();
	}

}


//---------------------------------------------------------

void CPhysicController::tick(unsigned int msecs) 
{
	// Llamar al método de la clase padre (IMPORTANTE).
	IComponent::tick(msecs);

	// Actualizar la posición y orientación de la entidad lógica usando la 
	// información proporcionada por el motor de física	
	_entity->setPosition(_server->getControllerPosition(_controller));

	// Intentamos mover el controller a la posición recibida en el último mensaje 
	// de tipo AVATAR_WALK. 
	unsigned flags = _server->moveController(_controller, _movement, msecs);

	// Actualizamos el flag que indica si estamos cayendo
	_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);

	//Mandamos un mensaje que dirá si hay collision con el suelo para la lógica
	Logic::CMessageCollisionDown *m=new Logic::CMessageCollisionDown();
	m->setCollisionDown(_falling);
	_entity->emitMessage(m);

	_movement = Vector3::ZERO;

}

//---------------------------------------------------------

void  CPhysicController::setPosition (const Vector3 &position){
	_server->setControllerPosition(_controller,position);
}


//---------------------------------------------------------

PxCapsuleController* CPhysicController::createController(const Map::CEntity *entityInfo)
{
	// Obtenemos la posición de la entidad. Inicialmente colocaremos el controller
	// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
	// lo atraviesa en el primer ciclo de simulación.
	Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);
	
	// Leer el volumen de colisión del controller. Por ahora sólo admitimos cápsulas.
	std::string shape = "capsule";
	if (entityInfo->hasAttribute("physic_shape")) {
		shape = entityInfo->getStringAttribute("physic_shape");
		assert(shape == "capsule");
	}

	// Leer el radio de la cápsula
	assert(entityInfo->hasAttribute("physic_radius"));
	float radius = entityInfo->getFloatAttribute("physic_radius");

	// Leer la altura de la cápsula
	assert(entityInfo->hasAttribute("physic_height"));
	float height = entityInfo->getFloatAttribute("physic_height");

	// Crear el controller de tipo cápsula
	return _server->createCapsuleController(position, radius, height, this);
} 

//---------------------------------------------------------

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter)
{

}

//---------------------------------------------------------

void CPhysicController::onShapeHit (const PxControllerShapeHit &hit)
{
	// Si chocamos contra una entidad estática no hacemos nada
	PxRigidDynamic* actor = hit.shape->getActor().isRigidDynamic();
	if(!actor)
		return;

	// Si chocamos contra una entidad cinemática no hacemos nada
	if (_server->isKinematic(actor))
		return;
	
	// Aplicar una fuerza a la entidad en la dirección del movimiento
	actor->addForce(hit.dir * hit.length * 1000.0f);
}

//---------------------------------------------------------

void CPhysicController::onControllerHit (const PxControllersHit &hit)
{

}

//---------------------------------------------------------



