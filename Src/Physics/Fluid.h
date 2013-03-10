//---------------------------------------------------------------------------
// Fluid.h
//---------------------------------------------------------------------------

/**
@file Fluid.h

@see Graphics::CFluid

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_Fluid_H
#define __Physics_Fluid_H

// Predeclaración de clases para ahorrar tiempo de compilación
namespace physx {
	class PxParticleFluid;
	class PxScene;
	class PxPhysics;
};

namespace Physics {
	/**
	
	@ingroup physicsGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	class CFluid {
	public:

		CFluid::CFluid(unsigned int maxParticles, float restitution, float viscosity,
				       float stiffness, float dynamicFriction, float particleDistance);

		virtual ~CFluid();

	protected:
		
		void createFluid(unsigned int maxParticles, float restitution, float viscosity,
					     float stiffness, float dynamicFriction, float particleDistance);

		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float msecs);

		/** Sistema de fluidos de PhysX. */
		physx::PxParticleFluid* _fluid;

		// Puntero a la escena
		physx::PxScene* _scene;

		// SDK de Physx
		physx::PxPhysics* _physxSDK;

		/** True si estamos usando CUDA para simular fluidos. */
		bool _runOnGPU;

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
