/**
@file CollisionManager.cpp

Contiene la implementación del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "CollisionManager.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Entity.h"

#include <PxRigidActor.h>
#include <PxShape.h> 

using namespace Logic;
using namespace physx;

namespace Physics {

	CCollisionManager::CCollisionManager() {
		// Nada que hacer
	}

	//________________________________________________________________________

	CCollisionManager::~CCollisionManager() {
		// Nada que hacer
	}

	//________________________________________________________________________

	void CCollisionManager::onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {
		// Por ahora ignoramos estos mensajes	
	}

	//________________________________________________________________________

	void CCollisionManager::onWake(PxActor **actors, PxU32 count) {
		// Por ahora ignoramos estos mensajes	
	}

	//________________________________________________________________________

	void CCollisionManager::onSleep(PxActor **actors, PxU32 count) {
		// Por ahora ignoramos estos mensajes	
	}

	//________________________________________________________________________

	void CCollisionManager::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) {
		// Para cada pareja de actores en los cuales se ha detectado contacto
		// comprobamos el tipo de contacto que se ha detectado y disparamos los
		// metodos correspondientes de IPhysics
		for(PxU32 i =0 ; i < nbPairs; ++i) {
			const PxContactPair& cp = pairs[i];

			// Nos aseguramos de que las shapes contra las que se ha detectado contacto no han
			// sido borradas
			if( cp.flags & (PxContactPairFlag::eDELETED_SHAPE_0 | PxContactPairFlag::eDELETED_SHAPE_0) )
				continue;

			// Comprobamos si acabamos de tocar o dejar de tocar el objeto
			bool enter = cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND;
			bool exit = cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST;

			if (!enter && !exit)
				continue;

			// Obtenemos los datos logicos asociados al primer actor
			IPhysics* firstActorBeingContacted = (IPhysics*) pairHeader.actors[0]->userData;
			assert(firstActorBeingContacted);

			// Obtenemos los datos logicos asociados al segundo actor
			IPhysics* secondActorBeingContacted = (IPhysics*) pairHeader.actors[1]->userData;
			assert(secondActorBeingContacted);

			// Disparamos los metodos onContact de la interfaz logica
			firstActorBeingContacted->onContact(secondActorBeingContacted, enter);
			secondActorBeingContacted->onContact(firstActorBeingContacted, enter);
		}
	}

	//________________________________________________________________________

	void CCollisionManager::onTrigger(PxTriggerPair *pairs, PxU32 count) {
		// Recorrer el array de colisiones
		for (unsigned int i = 0; i < count; ++i) {
		
			// Ignoramos los pares en los que alguna de las shapes (del trigger o de la otra entidad) ha sido borrada
			if( pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER) )
				continue;

			// Comprobamos si estamos saliendo o entrando en el trigger
			bool enter = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND;
			bool exit = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST;

			// Sólo tenemos en cuenta los eventos de entrada y salida del trigger. En PhysX 2.8 y anteriores
			// también se notificada si el objeto permanecía dentro del trigger. La documentación no deja muy
			// claro si este comportamiento se ha eliminado de manera definitiva.
			if (!enter && !exit)
				continue;

			// Obtenemos los datos lógicos asociados al trigger que se ha disparado
			IPhysics *triggerComponent = (IPhysics *) pairs[i].triggerShape->getActor().userData;
			assert(triggerComponent);

			// Obtenemos los datos lógicos asociados al actor que ha disparado el trigger
			IPhysics *otherComponent = (IPhysics *) pairs[i].otherShape->getActor().userData;
			assert(otherComponent);

			// Disparamos los metodos onTrigger de la interfaz logica
			triggerComponent->onTrigger(otherComponent, enter);
			otherComponent->onTrigger(triggerComponent, enter);
		}	
	}

	//________________________________________________________________________

	void CCollisionManager::onShapeHit(const PxControllerShapeHit &hit) {
		// Obtenemos los datos lógicos asociados al controller que ha golpeado la shape
		IPhysics* component = (IPhysics*) hit.controller->getUserData();
		// Obtenemos los datos lógicos asociados a la shape que el controller ha golpeado
		IPhysics* otherComponent = (IPhysics*)hit.shape->getActor().userData;

		// Disparamos los metodos onShapeHit de la interfaz logica
		component->onShapeHit(otherComponent);
		otherComponent->onShapeHit(component); 
	}

	//________________________________________________________________________

	void CCollisionManager::onControllerHit(const PxControllersHit &hit) {
		// Por ahora ignoramos estos mensajes	
	}

	//________________________________________________________________________

	void CCollisionManager::onObstacleHit(const PxControllerObstacleHit &hit) {
		// Por ahora ignoramos estos mensajes	
	}

}
