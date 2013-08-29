/**
@file CollisionManager.cpp

Contiene la implementación del gestor de colisiones.

@see Physics::CCollisionManager

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "CollisionManager.h"
#include "Conversions.h"
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
			if( cp.flags & (PxContactPairFlag::eDELETED_SHAPE_0 | PxContactPairFlag::eDELETED_SHAPE_1) )
				continue;

			// Comprobamos si acabamos de tocar o dejar de tocar el objeto
			// No hace falta comprobar mas flags porque solo hemos pedido que 
			// de estos dos nos notifiquen
			bool enter = cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND;
			bool exit = cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST;

			// El numero de contactos esta limitado al tamaño del buffer, solo
			// queremos recibir un punto de contacto
			unsigned int bufferSize = 1;
			PxContactPairPoint* contactBuffer = new PxContactPairPoint[bufferSize];
			unsigned int nbContacts = cp.extractContacts(contactBuffer, bufferSize);

			// Obtenemos los datos logicos asociados al primer actor
			IPhysics* firstActorBeingContacted = (IPhysics*) pairHeader.actors[0]->userData;
			assert(firstActorBeingContacted);

			// Obtenemos los datos logicos asociados al segundo actor
			IPhysics* secondActorBeingContacted = (IPhysics*) pairHeader.actors[1]->userData;
			assert(secondActorBeingContacted);

			// Disparamos los metodos onContact de la interfaz logica
			firstActorBeingContacted->onContact(secondActorBeingContacted, enter);
			secondActorBeingContacted->onContact(firstActorBeingContacted, enter);

			delete [] contactBuffer;
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
			// No hace falta comprobar mas flags porque solo hemos pedido que 
			// de estos dos nos notifiquen
			bool enter = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND;
			bool exit = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST;

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

		// Obtenemos información de la colisión que será relevante para la lógica
		// de momento solo nos interesa el punto de impacto y su normal
		Vector3 colisionPos = PxExtendedVec3ToVector3( hit.worldPos );
		Vector3 colisionNormal = PxVec3ToVector3( hit.worldNormal );

		// Disparamos los metodos onShapeHit de la interfaz logica
		component->onShapeHit(otherComponent, colisionPos, colisionNormal);
		otherComponent->onShapeHit(component, colisionPos, colisionNormal); 
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
