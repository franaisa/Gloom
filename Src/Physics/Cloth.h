//---------------------------------------------------------------------------
// Cloth.h
//---------------------------------------------------------------------------

/**
@file Cloth.h

@see Physics::CCloth

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_Cloth_H
#define __Physics_Cloth_H

#include <PxPhysicsAPI.h> // Cambiarlo!! Solo incluir lo necesario
#include <vector>

// Predeclaración de clases para ahorrar tiempo de compilación
/*namespace physx {
	class PxParticleFluid;
	class PxScene;
	class PxPhysics;
};*/

namespace Physics {
	
	/**
	
	CLASE GUARRA PARA HACER PRUEBAS DE TELAS

	@ingroup physicsGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	
	class CCloth {
	public:

		CCloth::CCloth();

		virtual ~CCloth();

	protected:
		
		void createCloth();

		physx::PxClothFabric* createFabric(const physx::PxClothMeshDesc& clothDesc, const physx::PxVec3& gravityDir);

		physx::PxClothMeshDesc createCustomClothDescriptor(int width, int height, float d);

		void createCustomClothCollisionData(physx::PxClothFabric* fabric, const physx::PxClothMeshDesc& clothDesc, 
									        int width, int height, float d);

		// Puntero a la escena
		physx::PxScene* _scene;

		// SDK de Physx
		physx::PxPhysics* _physxSDK;

		physx::PxCooking* _cooking;

		// Mierda variada
		physx::PxCloth* _cloth;
		physx::PxClothCollisionSphere _box_collider;
		std::vector<physx::PxVec3> _pos;
		std::vector<physx::PxVec3> _normal;
		std::vector<physx::PxU32> _indices;

	}; // class CCloth

} // namespace Physics

#endif // __Physics_Cloth_H
