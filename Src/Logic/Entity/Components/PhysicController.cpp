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
	// TODO: recibir mensajes de tipo AVATAR_WALK
	return false;
} 

//---------------------------------------------------------

void CPhysicController::process(CMessage *message)
{
	switch(message->getMessageType())
	{
		// TODO: Procesar mensajes de tipo AVATAR_WALK
		// Anotamos el vector de desplazamiento recibido en el mensaje en el atributo
		// privado _movement para aplicar el movimiento en el tick
	}

} 

//---------------------------------------------------------

void CPhysicController::tick(unsigned int msecs) 
{
	// Llamar al método de la clase padre (IMPORTANTE).
	IComponent::tick(msecs);

	// TODO: actualizar la posición de la entidad lógica
	// 1. Recuperar la posición del controller físico usando el servidor
	// 2. Actualizar la posición de la entidad física

	// TODO: simular la gravedad 
	// Si el controller está cayendo sumar el vector (0, -1, 0) al desplazamiento

	// TODO: pedir al controller que se mueva
	// 1. Mover el controller usando el servidor de física
	
	// TODO: comprobar si el controller toca el suelo o está cayendo
	// Comprobar si está activo el flag PxControllerFlag::eCOLLISION_DOWN en la
	// máscara de bits que devuelve el método que mueve el controller, y actualizar
	// el valor de _falling

	// Ponemos el movimiento a cero
	_movement = Vector3::ZERO;
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

	// TODO: Crear el controller
	// 1. Leer radio y altura de la capsula
	// 2. Crear el controller usando el servidor de física (pasar como componente this)
	return NULL;
} 

//---------------------------------------------------------

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter)
{

}

//---------------------------------------------------------

void CPhysicController::onShapeHit (const PxControllerShapeHit &hit)
{
	// TODO: aplicar una fuerza a la entidad dinámica contra la que chocamos
	// 1. Obtener el actor del objeto contra el que hemos colisionado
	// 2. Si es un actor estático no hacemos nada (usar método de PxActor para comprobar)
	// 3. Si es un actor cinemático no hacemos nada (usar método de CServer para comprobar)
	// 4. En otro caso aplicamos una fuerza a la entidad en la dirección del golpe
}

//---------------------------------------------------------

void CPhysicController::onControllerHit (const PxControllersHit &hit)
{

}

//---------------------------------------------------------



