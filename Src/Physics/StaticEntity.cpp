//---------------------------------------------------------------------------
// StaticEntity.cpp
//---------------------------------------------------------------------------

/**
@file StaticEntity.cpp

@see Physics::CStaticEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "StaticEntity.h"

#include <assert.h>

#include "Physics/Server.h"
#include "Physics/CollisionManager.h"
#include "Conversions.h"

#include "Logic/Entity/Components/Physics.h"

#include <PxPhysics.h>
#include <PxScene.h>
#include <PxShape.h>
#include <PxRigidActor.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxSimpleFactory.h>
#include <geometry/PxGeometryHelpers.h>

using namespace physx;
using namespace std;

namespace Physics {

	void CStaticEntity::load(const Vector3 &position, const PxGeometry& geometry, PxMaterial& material, 
					         bool trigger, int group, const vector<int>& groupList, const Logic::IPhysics *component) {

		assert(_scene);

		_isTrigger = trigger;
						
		// Creamos una esfera estática
		PxTransform globalPose(Vector3ToPxVec3(position));
		
		// Transformación de coordenadas lógicas a coodenadas de PhysX
		PxTransform shapeOffset( PxVec3(0, getLogicPivotOffset(geometry), 0) );
		
		_actor = PxCreateStatic(*_physxSDK, globalPose, geometry, material, shapeOffset);

		// Transformarlo en trigger si es necesario
		if (trigger) {
			PxShape *shape;
			_actor->getShapes(&shape, 1, 0);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		// Asociar el componente lógico asociado a la entidad física
		_actor->userData = (void*) component;

		// Establecer el grupo de colisión
		PxSetGroup(*_actor, group);
		// Establecer los filtros en base al grupo de colision
		Physics::CServer::getSingletonPtr()->setupFiltering(_actor, group, groupList);

		// Añadir el actor a la escena
		_scene->addActor(*_actor);
	}

	//________________________________________________________________________

	void CStaticEntity::load(const PxPlane& plane, PxMaterial& material, int group, 
							 const vector<int>& groupList, const Logic::IPhysics *component) {

		assert(_scene);

		// Crear un plano estático
		_actor = PxCreatePlane(*_physxSDK, plane, material);
	
		// Anotar el componente lógico asociado a la entidad física
		_actor->userData = (void*) component;

		// Establecer el grupo de colisión
		PxSetGroup(*_actor, group);

		Physics::CServer::getSingletonPtr()->setupFiltering(_actor, group, groupList);
	
		// Añadir el actor a la escena
		_scene->addActor(*_actor);
	}

	//________________________________________________________________________

	void CStaticEntity::load(const string &file, int group, const vector<int>& groupList, const Logic::IPhysics *component) {
		// Deserializamos el fichero RepX
		_actor = deserializeFromRepXFile(file, group, groupList, component);

		// Asignamos al actor los flags que corresponden a los rigid estaticos
	}

} // namespace Physics
