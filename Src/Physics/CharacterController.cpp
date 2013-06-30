//---------------------------------------------------------------------------
// CharacterController.cpp
//---------------------------------------------------------------------------

/**
@file CharacterController.cpp

@see Physics::CCharacterController
@see physx::PxController

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "CharacterController.h"

#include <assert.h>

#include "Physics/Server.h"
#include "Physics/CollisionManager.h"
#include "Conversions.h"

#include "Logic/Entity/Components/PhysicController.h"

#include <PxPhysics.h>
#include <PxScene.h>
#include <PxShape.h>
#include <PxRigidDynamic.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <characterkinematic/PxCapsuleController.h>
#include <characterkinematic/PxController.h>
#include <characterkinematic/PxControllerManager.h>

using namespace physx;

namespace Physics {

	CCharacterController::CCharacterController() {
		// Obtenemos la sdk de physics y comprobamos que ha sido inicializada
		_physxSDK = Physics::CServer::getSingletonPtr()->getPhysxSDK();
		assert(_physxSDK && "No se ha inicializado physX");
		
		// Obtenemos la escena de physx y comprobamos que ha sido inicializada
		_scene = Physics::CServer::getSingletonPtr()->getActiveScene();
		assert(_scene && "No existe una escena fisica");

		// Obtenemos el manager de controladores y comprobamos que ha sido inicializado
		_controllerManager = Physics::CServer::getSingletonPtr()->getControllerManager();
		assert(_controllerManager && "No existe controller manager");

		// Obtenemos el gestor de colisiones
		_collisionManager = Physics::CServer::getSingletonPtr()->getCollisionManager();
	} // CCharacterController

	//________________________________________________________________________

	CCharacterController::~CCharacterController() {
		// Destruimos el actor de physx asociado al controller. El gestor de controladores
		// en su release ya se encarga de desligar el controlador de la escena.
		if(_controller != NULL) {
			_controller->release();
			_controller = NULL;
		}

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
		desc.contactOffset = 0.5f;
		desc.height = height;
		desc.radius = radius;
		desc.material = _physxSDK->createMaterial(0.5f, 0.5f, 0.1f); // En realidad sera getDefaultMaterial en el futuro
		desc.climbingMode = PxCapsuleClimbingMode::eEASY;
		
		// Indicamos que queremos asignar nuestro propio filtro de colisiones
		// para el player
		desc.interactionMode = PxCCTInteractionMode::eUSE_FILTER;
		desc.groupsBitmask = (1 << group);

		_filterMask = groupList.empty() ? 0 : ( groupList[0] == -1 ? 0xFFFFFFFF : (1 << groupList[0]) );
		// Fijamos la mascara de colisiones para el movimiento
		for(unsigned int i = 1; i < groupList.size(); ++i) {
			_filterMask |= ( 1 << groupList[i] );
		}

		//desc.contactOffset=0.001f; //<-- Tamaño que recubre el volumen de la capsula, physx lo usa para no permitir que la capsula se introduzca en otros shapes
		//desc.stepOffset <-- Tamaño de los escalones que el player puede subir
		//desc.slopeLimit = 0.707f;

		desc.callback = _collisionManager;   // Establecer gestor de colisiones
		desc.userData = (void *) component;  // Anotar el componente lógico asociado al controller

		// Retorna un PxController que podemos castear a capsule controller por ser nuestro caso
		_controller = static_cast<PxCapsuleController*>( _controllerManager->createController(*_physxSDK, _scene, desc) );

		// Anotar el componente lógico asociado al actor dentro del controller (No es automático)
		_controller->getActor()->userData = (void *) component;

		// Establecemos el filtro de colision para el resto de shapes que no son players
		Physics::CServer::getSingletonPtr()->setupFiltering(_controller->getActor(), group, groupList);

		//Orientacion del jugador (Por si algun dia hiciera falta
		/*PxRigidDynamic* actor = _controller->getActor(); // get the actor
		PxShape* shapes[1]; // There is only one shape in this controller
		actor->getShapes(shapes,1,0); // get that shape
		PxShape* shape = shapes[0]; // Here is your shape
		PxReal yawValue = [yourMeshOrientationHere] // the values must be in radian
		shape->setLocalPose(PxTransform(PxQuat(yawValue,PxVec3(1,0,0)))); // turns the PxShape around yaw ( y axis )*/
	}

	//________________________________________________________________________

	unsigned CCharacterController::move(const Vector3 &movement, unsigned int msecs) {
		// Vector de desplazamiento, nosotros somos los encargados de sumarle la fuerza
		// de la gravedad, ya que physX no lo hace por nosotros.
		PxVec3 disp = Vector3ToPxVec3(movement);

		// Garantiza que los players no se solapen. Empuja las capsulas cuando se van a 
		// solapar.
		_controllerManager->computeInteractions(msecs);

		// Movemos el character controller y retornamos un entero con los flags de colision (PxControllerFlag)
		// Fijamos la distancia minima a la que parar el algoritmo de movimiento a 0.01f.
		// Pasamos el tiempo de frame transcurrido en micro segundos (como a physX le gusta).
		// Dado que no tenemos objetos fisicos no manejados por physX, pasamos NULL como obstaculo.
		PxControllerFilters filters(_filterMask);
		return _controller->move(disp, 0.01f, msecs * 0.001f, filters, NULL);
	}

	//________________________________________________________________________

	unsigned CCharacterController::move(const Vector3& movement, unsigned int customFilterMask, unsigned int msecs) {
		PxVec3 disp = Vector3ToPxVec3(movement);
		PxControllerFilters filters(customFilterMask);
		return _controller->move(disp, 0.01f, msecs * 0.001f, filters, NULL);
	}

	//________________________________________________________________________

	Vector3 CCharacterController::getPosition() {
		// Antes de devolver la posición del controller debemos transformar entre el 
		// sistema de coordenadas de PhysX y el de la lógica
		float offsetY = ( _controller->getHeight() * 0.5f ) + _controller->getRadius();
		Vector3 pos = PxExtendedVec3ToVector3( _controller->getPosition() );
		
		return pos - Vector3(0, offsetY, 0);
	}

	//________________________________________________________________________

	void CCharacterController::setPosition(const Vector3& position) {
		// Transformación entre el sistema de coordenadas lógico y el de PhysX
		float offsetY = ( _controller->getHeight() * 0.5f ) + _controller->getRadius();
		PxVec3 pos = Vector3ToPxVec3(position + Vector3(0, offsetY, 0));

		// Transformamos el vector a uno de mayor precision que physx utiliza por debajo
		// para manejar a los controllers con mayor exactitud en mundos muy grandes
		_controller->setPosition( PxExtendedVec3(pos.x, pos.y, pos.z) );
	}

	//__________________________________________________________________

	float CCharacterController::getCapsuleRadius() {
		return _controller->getRadius();
	}

	//__________________________________________________________________

	float CCharacterController::getCapsuleHeight() {
		return _controller->getHeight();
	}

	//________________________________________________________________________

	void CCharacterController::activateSimulation() {
		// Activamos todos los shapes del componente por completo en PhysX
		// Para ello, obtenemos todos sus shapes y ponemos los flags a true

		int nbShapes = _controller->getActor()->getNbShapes();
		PxShape** actorShapes = new PxShape* [nbShapes];
		_controller->getActor()->getShapes(actorShapes, nbShapes);
		for(int i = 0; i < nbShapes; ++i) {
			// Esta shape tomara parte en barridos, raycasts...
			actorShapes[i]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
			// Esta shape entrara dentro de la simulacion de fisicas
			actorShapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE , true);
		}
		// Hace que el controlador colisione con los grupos asignados
		_controller->setInteraction(PxCCTInteractionMode::eUSE_FILTER);

		delete [] actorShapes;
	}

	//________________________________________________________________________

	void CCharacterController::deactivateSimulation() {
		// Desactivamos todos los shapes del componente por completo en PhysX
		// Para ello, obtenemos todos sus shapes y ponemos los flags a false

		int nbShapes = _controller->getActor()->getNbShapes();
		PxShape** actorShapes = new PxShape* [nbShapes];
		_controller->getActor()->getShapes(actorShapes, nbShapes);
		for(int i = 0; i < nbShapes; ++i) {
			// Esta shape no tomara parte en barridos, raycasts...
			actorShapes[i]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
			// Esta shape no entrara dentro de la simulacion de fisicas
			actorShapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE , false);
		}
		// Hace que el controlador no colisione con nada
		_controller->setInteraction(PxCCTInteractionMode::eEXCLUDE);

		delete [] actorShapes;
	}

} // namespace Physics
