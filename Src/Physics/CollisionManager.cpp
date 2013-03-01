/**
@file CollisionManager.cpp

Contiene la implementación del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "CollisionManager.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Components/PhysicEntity.h"
#include "Logic/Entity/Entity.h"

#include <PxRigidActor.h>
#include <PxShape.h> 
#include <PxSimulationEventCallback.h> 
#include <PxPhysicsAPI.h>

using namespace Physics;
using namespace Logic;
using namespace physx;

//--------------------------------------------------

CCollisionManager::CCollisionManager() {
	for(int i = 0; i < 32; ++i) {
		_collisionGroupLookupTable[i] = NULL;
	}
}

//--------------------------------------------------

CCollisionManager::~CCollisionManager() {
	for(int i = 0; i < 32; ++i) {
		if(_collisionGroupLookupTable[i] != NULL) {
			// Borramos aquellos sets que hayan sido inicializados
			delete _collisionGroupLookupTable[i];
		}
	}
}

//--------------------------------------------------

void CCollisionManager::onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onWake(PxActor **actors, PxU32 count) {
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onSleep(PxActor **actors, PxU32 count) {
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) {
	// Para cada pareja de actores en los cuales se ha detectado contacto
	// comprobamos el tipo de contacto que se ha detectado y disparamos los
	// metodos correspondientes de IPhysics
	for(PxU32 i =0 ; i < nbPairs; ++i) {
		const PxContactPair& cp = pairs[i];

		// eNOTIFY_TOUCH_FOUND es un flag para indicar que los shapes acaban de empezar a estar en contacto
		// de momento solo necesitamos saber cuando colisionan dos shapes. En un futuro
		// puede interesarnos cuando dos shapes y entran en contacto y dejan de estarlo.
		if(cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			// Obtenemos los datos logicos asociados al primer actor
			IPhysics* firstActorBeingContacted = (IPhysics *) pairHeader.actors[0]->userData;
			assert(firstActorBeingContacted);

			// Obtenemos los datos logicos asociados al segundo actor
			IPhysics* secondActorBeingContacted = (IPhysics *) pairHeader.actors[1]->userData;
			assert(secondActorBeingContacted);

			// Disparamos los metodos onContact de la interfaz logica
			firstActorBeingContacted->onContact(secondActorBeingContacted);
			secondActorBeingContacted->onContact(firstActorBeingContacted);
		}
	}
}

//--------------------------------------------------

void CCollisionManager::onTrigger(PxTriggerPair *pairs, PxU32 count) {
	// Recorrer el array de colisiones
	for (unsigned int i = 0; i < count; ++i) {
		
		// Ignoramos los pares en los que alguna de las shapes (del trigger o de la otra entidad) ha sido borrada
		if (pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
			continue;

		PxU16 group1 = PxGetGroup(pairs[i].triggerShape->getActor());
		PxU16 group2 = PxGetGroup(pairs[i].otherShape->getActor());

		// Si no se han activado las colisiones entre estos dos grupos para estos triggers
		// no hacer nada
		if ( !collisionEnabled(group1, group2) )
			continue;

		// Comprobamos si estamos saliendo o entrando en el trigger
		bool enter = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND;
		bool exit = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST;

		// Sólo tenemos en cuenta los eventos de entrada y salida del trigger. En PhysX 2.8 y anteriores
		// también se notificada si el objeto permanecía dentro del trigger. La documentación no deja muy
		// claro si este comportamiento se ha eliminado de manera definitiva.
		if (!enter && !exit)
			continue;

		// Notificar a los componentes físicos de la lógica las colisiones
		// 1. Obtener el componente lógico (IPhysics) asociado al trigger físico.
		// 2. Obtener el componente lógico (IPhysics) asociado a la otra entidad física.
		// 3. Notificar a ambos componentes la colisión
		IPhysics *triggerComponent = (IPhysics *) pairs[i].triggerShape->getActor().userData;
		assert(triggerComponent);

		IPhysics *otherComponent = (IPhysics *) pairs[i].otherShape->getActor().userData;
		assert(otherComponent);

		triggerComponent->onTrigger(otherComponent, enter);
		otherComponent->onTrigger(triggerComponent, enter);
	}	
}

//--------------------------------------------------

void CCollisionManager::onShapeHit(const PxControllerShapeHit &hit) {
	// TODO: notificar al componente físico la colisión con una entidad
	// 1. Obtener el puntero al componente físico (CPhysicController)
	// 2. Notificar la colisión al componente físico
	CPhysicController * component = (CPhysicController *) hit.controller->getUserData();
	component->onShapeHit(hit);
}

//--------------------------------------------------

void CCollisionManager::onControllerHit(const PxControllersHit &hit) {
	// TODO: notificar al componente físico la colisión con otro controller
	// 1. Obtener el puntero al componente físico (CPhysicController)
	// 2. Notificar la colisión al componente físico
	
}

//--------------------------------------------------

void CCollisionManager::onObstacleHit(const PxControllerObstacleHit &hit) {
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::setCollisionGroup(physx::PxU16 group1, physx::PxU16 group2, bool enabled) {
	// Para insertar un nuevo grupo de colision comprobamos si el set habia sido inicializado. Si no
	// habia sido inicializado, reservamos memoria e insertamos el grupo de colision.
	if(enabled) {
		if(_collisionGroupLookupTable[group1] != NULL) {
			_collisionGroupLookupTable[group1]->insert(group2);
		}
		else {
			_collisionGroupLookupTable[group1] = new(std::nothrow) std::set<physx::PxU16>;
			assert(_collisionGroupLookupTable[group1] != NULL && "Error: No se puede reservar memoria para el grupo de colision");
		}

		if(_collisionGroupLookupTable[group2] != NULL) {
			_collisionGroupLookupTable[group2]->insert(group1);
		}
		else {
			_collisionGroupLookupTable[group2] = new(std::nothrow) std::set<physx::PxU16>;
			assert(_collisionGroupLookupTable[group2] != NULL && "Error: No se puede reservar memoria para el grupo de colision");
		}
	}
	// Eliminamos el grupo de colision del set solo si el set ha sido inicializado.
	else {
		if(_collisionGroupLookupTable[group1] != NULL) {
			_collisionGroupLookupTable[group1]->erase(group2);
		}
		
		if(_collisionGroupLookupTable[group2] != NULL) {
			_collisionGroupLookupTable[group2]->erase(group1);
		}
	}
}

//--------------------------------------------------

bool CCollisionManager::collisionEnabled(PxU16 group1, PxU16 group2) const {
	// true si existe un elemento group2 en el set de group1
	if(_collisionGroupLookupTable[group1] != NULL)
		return _collisionGroupLookupTable[group1]->count(group2) != 0;
	else
		return true;
}