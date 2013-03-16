//---------------------------------------------------------------------------
// CharacterController.cpp
//---------------------------------------------------------------------------

/**
@file CharacterController.cpp

@see Graphics::CCharacterController

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "CharacterController.h"

#include <assert.h>

#include "Physics/Server.h"
#include "Physics/CollisionManager.h"
#include "Conversions.h"

#include <PxPhysicsAPI.h> // Cambiarlo!! Solo incluir lo necesario

using namespace physx;

namespace Physics {

	CCharacterController::CCharacterController() {
		// Obtenemos la sdk de physics y comprobamos que ha sido inicializada
		_physxSDK = Physics::CServer::getSingletonPtr()->getPhysxSDK();
		assert(_physxSDK && "No se ha inicializado physX");
		
		// Obtenemos la escena de physx y comprobamos que ha sido inicializada
		_scene = Physics::CServer::getSingletonPtr()->getActiveScene();
		assert(_scene && "No existe una escena fisica");

		_controllerManager = Physics::CServer::getSingletonPtr()->getControllerManager();
		assert(_controllerManager && "No existe controller manager");

		_collisionManager = Physics::CServer::getSingletonPtr()->getCollisionManager();
	} // CCharacterController

	//________________________________________________________________________

	CCharacterController::~CCharacterController() {
		// Destruimos el actor de physx asociado al controller y desligamos el 
		// actor de la escena

		// Fijamos los punteros a physx como nulos
		_physxSDK = NULL;
		_scene = NULL;
		_controllerManager = NULL;
		_collisionManager = NULL;
	} // ~CCharacterController

	//________________________________________________________________________

	void CCharacterController::load(const Vector3 &position, float radius, float height, 
									int group, const std::vector<int>& groupList, 
									const Logic::CPhysicController* component) {

		assert(_scene);

		// Nota: PhysX coloca el sistema de coordenadas local en el centro de la cápsula, mientras
		// que la lógica asume que el origen del sistema de coordenadas está en los piés del 
		// jugador. Para unificar necesitamos realizar una traslación en el eje Y.
		// Desafortunadamente, el descriptor que se usa para crear los controllers no permite
		// definir esta transformación local (que sí permite al crear un actor), por lo que
		// tendremos que realizar la traslación nosotros cada vez. 

		// Transformación entre el sistema de coordenadas lógico y el de PhysX
		float offsetY = height / 2.0f + radius;
		PxVec3 pos = Vector3ToPxVec3(position + Vector3(0, offsetY, 0));
	
		// Crear descriptor del controller
		PxCapsuleControllerDesc desc;
		desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
		desc.height = height;
		desc.radius = radius;
		desc.material = _physxSDK->createMaterial(0.5f, 0.5f, 0.1f); // En realidad sera getDefaultMaterial en el futuro
		desc.climbingMode = PxCapsuleClimbingMode::eEASY;
		//desc.slopeLimit = 0.707f;
		desc.callback = _collisionManager;   // Establecer gestor de colisiones
		desc.userData = (void *) component;  // Anotar el componente lógico asociado al controller

		// Retorna un PxController que podemos castear a capsule controller por ser nuestro caso
		_controller = static_cast<PxCapsuleController*>( _controllerManager->createController(*_physxSDK, _scene, desc) );
	
		// Anotar el componente lógico asociado al actor dentro del controller (No es automático)
		_controller->getActor()->userData = (void *) component;

		// Establecer el grupo de colisión
		PxSetGroup(*_controller->getActor(), group);
		Physics::CServer::getSingletonPtr()->setupFiltering(_controller->getActor(), group, groupList);
	}

	//________________________________________________________________________
		
	void CCharacterController::tick(float msecs) {
	} // tick

} // namespace Physics
