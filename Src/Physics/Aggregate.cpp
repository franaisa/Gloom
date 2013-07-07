//---------------------------------------------------------------------------
// Aggregate.cpp
//---------------------------------------------------------------------------

/**
@file Aggregate.cpp

@see Physics::CAggregate

@author Francisco Aisa García
@date Julio, 2013
*/

#include "Aggregate.h"
#include "Entity.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "Conversions.h"
#include "Physics/Server.h"
#include "Physics/CollisionManager.h"
#include "Logic/Entity/Components/Physics.h"

#include <assert.h>

#include <PxPhysics.h>
#include <PxScene.h>
#include <PxShape.h>
#include <PxRigidActor.h>
#include <PxArticulationJoint.h>
#include <PxAggregate.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include <PxStringTable.h>
#include <cooking/PxCooking.h>
#include <extensions/PxD6Joint.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxSimpleFactory.h>
#include <extensions/PxDefaultStreams.h>
#include <extensions/PxStringTableExt.h>
#include <geometry/PxGeometryHelpers.h>
#include <RepX/RepXUtility.h>

using namespace physx;
using namespace std;

namespace Physics {

	CAggregate::CAggregate() : _aggregate(NULL) {
		// Nada que hacer
	} // CAggregate

	//________________________________________________________________________

	CAggregate::~CAggregate() {
		if(_aggregate != NULL) {
			if( !_actors.empty() ) {
				for(int i = 0; i < _actors.size(); ++i) {
					delete _actors[i];
				}

				_actors.clear();
			}

			Physics::CServer::getSingletonPtr()->destroyAggregate(_aggregate);
			_aggregate = NULL;
		}
	} // ~CAggregate

	//________________________________________________________________________

	void CAggregate::load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component, bool nameActors) {
		// Obtenemos el puntero al servidor de fisicas
		Physics::CServer* physicsServer = Physics::CServer::getSingletonPtr();
		// Obtenemos la sdk de physics y comprobamos que ha sido inicializada
		PxPhysics* physxSDK = physicsServer->getPhysxSDK();
		// Obtenemos la clase de cocinado de physX y comprobamos que ha sido inicializada
		PxCooking* cooking = physicsServer->getCooking();

		// Preparar parámetros para deserializar
		PxDefaultFileInputData data(file.c_str());
		PxCollection* bufferCollection = physxSDK->createCollection();
		PxCollection* sceneCollection = physxSDK->createCollection();
		PxStringTable* stringTable = nameActors ? &PxStringTableExt::createStringTable( CServer::getSingletonPtr()->getFoundation()->getAllocatorCallback() ) : NULL;
		PxUserReferences* externalRefs = NULL; 
		PxUserReferences* userRefs = NULL; 

		// Deserializar a partir del fichero RepX
		repx::deserializeFromRepX(data, *physxSDK, *cooking, stringTable, externalRefs, 
								  *bufferCollection, *sceneCollection, userRefs);

		// Obtenemos el número de actores que debemos cargar
		PxU32 nbActors = sceneCollection->getNbObjects();

		deserializeAggregate(sceneCollection, nbActors, component, group, groupList);

		// Liberar recursos
		bufferCollection->release();
		sceneCollection->release();
	}

	//________________________________________________________________________

	const std::vector<Physics::CDynamicEntity*> CAggregate::getEntities() {
		return _actors;
	}

	//________________________________________________________________________

	void CAggregate::deserializeAggregate(PxCollection* sceneCollection, unsigned int nbActors, const Logic::IPhysics* component, int group, const std::vector<int>& groupList) {
		// Creamos un agregado con el numero maximo de actores leidos y activamos
		// las colisiones para el propio ragdoll (selfcollisions = true)
		_aggregate = Physics::CServer::getSingletonPtr()->getPhysxSDK()->createAggregate(nbActors, true);

		// Reservamos memoria para el numero de actores que va a contener el agregado
		_actors.reserve(nbActors);

		PxPhysics* physxSDK = Physics::CServer::getSingletonPtr()->getPhysxSDK();

		// Asumimos que los datos contenidos en el fichero corresponden a los colliders
		// y articulaciones de un ragdoll
		PxSerializable* serializable;
		PxActor* actor = NULL;
		PxRigidActor* rigid = NULL;
		
		PxArticulation* articulation;
		PxArticulationJoint* artJoint;
		PxD6Joint* joint;
		
		for (unsigned int i = 0; i < nbActors; ++i) {
			serializable = sceneCollection->getObject(i);
			
			if( joint = serializable->is<PxD6Joint>() ) {
				//cout << "joint = " << joint->getName() << endl;
				// Crear una articulacion a partir del joint
				//_aggregate->addArticulation(articulation);
			}

			// Las articulaciones también son actores
			if( actor = serializable->is<PxActor>() ) {
				// Añadimos el actor al conjunto de actores del ragdoll
				_aggregate->addActor(*actor);
				// Anotamos el componente lógico asociado a la entidad física
				actor->userData = (void*)component;

				// Si es un collider y no una articulación (hitbox) le asignamos
				// los filtros de colisión que correspondan
				if( rigid = serializable->is<PxRigidActor>() ) {
					// Establecemos el grupo de colision
					PxSetGroup(*rigid, group);
					// Establecemos los filtros de colisión
					Physics::CServer::getSingletonPtr()->setupFiltering(rigid, group, groupList);
					// Creamos una nueva entidad a partir del actor de physx dado
					if( rigid->isRigidDynamic() )
						_actors.push_back( new Physics::CDynamicEntity(rigid) );
				}
			}
		}

		// Añadimos el agregado a la escena
		Physics::CServer::getSingletonPtr()->getActiveScene()->addAggregate(*_aggregate);
		
	}

	//________________________________________________________________________

	void CAggregate::activateSimulation() {
		if(_aggregate != NULL)
			for(int i = 0; i < _actors.size(); ++i)
				_actors[i]->activateSimulation();
	}

	//________________________________________________________________________

	void CAggregate::deactivateSimulation() {
		if(_aggregate != NULL)
			for(int i = 0; i < _actors.size(); ++i)
				_actors[i]->deactivateSimulation();
	}

} // namespace Physics
