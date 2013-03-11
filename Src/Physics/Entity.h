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

		/** Constructor por defecto. */
		CFluid();

		/** Destructor de la aplicación. */
		virtual ~CFluid();

		/**
		Añade la entidad al SceneManager pasado por parámetro. Si la entidad
		no está cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere añadir
		la entidad.
		@return true si la entidad se pudo cargar y añadir a la escena.
		*/
		//bool attachToScene(CScene *scene);

	protected:

		// CScene es la única que puede añadir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y eliminó de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga la entidad gráfica correspondiente al nombre _mesh. No hace 
		comprobaciónes de si la entidad está ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si la entidad está o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		physx::PxParticleFluid* createFluid(unsigned int maxParticles, float restitution, float viscosity,
								            float stiffness, float dynamicFriction, float particleDistance);

		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);
		
		/**
		Nodo que contiene la entidad de Ogre.
		*/
		//Ogre::SceneNode *_entityNode;

		/** Sistema de fluidos de PhysX. */
		physx::PxParticleFluid* _fluid;

		/** @deprecated De momento tenemos la escena de physx, en un futuro sera CScene. */
		physx::PxScene* _scene;

		// SDK de Physx
		/** 
		@deprecated De momento mantenemos el puntero a la SDK, pero en el futuro se deberia
		llamar al server para que haga lo que tenga que hacer.
		*/
		physx::PxPhysics* _physics;

		/**
		Indica si la entidad ha sido cargada en el motor gráfico.
		*/
		bool _loaded;

		bool _runOnGPU;

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
