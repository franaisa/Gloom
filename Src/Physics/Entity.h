//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaración de la clase que encapsula las operaciones 
básicas de los rigid bodies.

@see Physics::CEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_Entity_H
#define __Physics_Entity_H

#include "BaseSubsystems/Math.h"

#include <geometry/PxGeometry.h>
#include <PxMaterial.h>
#include <PxForceMode.h>

#include <vector>
#include <string>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace physx {
	class PxScene;
	class PxPhysics;
	class PxRigidActor;
	class PxCooking;
	class PxCollection;
	class PxAggregate;
}

namespace Physics {
	class CCollisionManager;
}

namespace Logic {
	class IPhysics;
}

namespace Physics {

	/**
	Esta clase implementa la funcionalidad básica de los rigid bodies.
	Las entidades dinámicas y estáticas tendrán que derivar de ésta.

	@ingroup physicsGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CEntity {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CEntity::CEntity();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CEntity();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Método virtual puro que debe ser implementado por las clases hijas para 
		deserializar los datos físicos desde un fichero.

		Contiene una implementación por defecto que las clases hijas pueden 
		reutilizar.

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisión que queremos asignar al actor.
		@param groupList Grupos de colisión con los que el actor quiere interactuar.
		@param component Componente lógico asociado.
		@param nameActors true si queremos que los colliders tengan un nombre asignado.
		*/
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component, bool nameActors = false) = 0;

		//________________________________________________________________________

		/** Devuelve la posición y rotación de la entidad física. */
		Matrix4 getTransform() const;

		//________________________________________________________________________

		/** Devuelve la posición de la entidad fisica*/
		Vector3 getPosition() const;

		//________________________________________________________________________

		/** Devuelve la rotación de la entidad física. */
		Quaternion getOrientation() const;

		//________________________________________________________________________

		/** Activa la simulación física. */
		void activateSimulation();

		//________________________________________________________________________

		/** Desactiva la simulación física. */
		void deactivateSimulation();


	protected:


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


		/** 
		Método para construir un actor de PhysX a partir de un fichero RepX. Si existen
		varios actores en el fichero se cargarán como un agregado (se asume que todos los
		actores serán usados en una misma entidad).

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisión que queremos asignar al actor.
		@param groupList Grupos de colisión con los que el actor quiere interactuar.
		@param component Componente lógico asociado.
		@param nameActors true si queremos que se le asigne nombres a los colliders.
		*/
		void deserializeFromRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component,
									 bool nameActors);

		//________________________________________________________________________

		/**
		Dada una colección de elementos, deserializa todos sus actores en un agregado.

		Normalmente usaremos esta función para deserializar ragdolls.

		@param sceneCollection Colección de elementos.
		@param nbActors Número de actores en la colección.
		@param component Componente lógico asociado a cada uno de los actores del agregado.
		@param group Grupo de colisión del agregado.
		@param Grupos de colisión con los que el agregado debe interactuar.
		*/
		void deserializeAggregate(physx::PxCollection* sceneCollection, unsigned int nbActors, const Logic::IPhysics* component, int group, const std::vector<int>& groupList);

		//________________________________________________________________________

		/**
		Dada una colección de elementos, deserializa un solo actor. Se usa cuando se sabe
		que solo existe un actor en el fichero que vamos a deserializar.

		@param sceneCollection Colección de elementos.
		@param component Componente lógico asociado al actor que se va a deserializar.
		@param group Grupo de colisión del actor.
		@param Grupos de colisión con los que el actor debe interactuar.
		*/
		void deserializeActor(physx::PxCollection* sceneCollection, const Logic::IPhysics* component, int group, const std::vector<int>& groupList);

		//________________________________________________________________________

		/**
		Dado un actor, se activa su simulación física.

		@param actor Actor que va a ser activado.
		*/
		void activateSimulation(physx::PxActor* actor);

		//________________________________________________________________________

		/**
		Dado un actor, se desactiva su simulación física.

		@param actor Actor que va a ser desactivado.
		*/
		void deactivateSimulation(physx::PxActor* actor);

		//________________________________________________________________________

		/**
		Dada una geometría devuelve el desfase que existe entre el pivote lógico y
		el físico, ya que en la física el pivote se encuentra en el centro del objeto
		y en la lógica en el pie.

		@param geometry Geometria a la cual queremos calcularle el desfase del pivote.
		@return Defase entre el pivote lógico y el físico (solo afecta a la coordenada
		y).
		*/
		float getLogicPivotOffset(const physx::PxGeometry& geometry);


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Puntero al actor de PhysX. Importante: Tiene que ser inicializado por la clase hija. */
		physx::PxRigidActor* _actor;

		/** Agregado que contiene todos los actores leidos desde fichero (si es que hay más de uno). */
		physx::PxAggregate* _aggregate;

		/** Puntero a la escena de PhysX. */
		physx::PxScene* _scene;

		/** Puntero al core de PhysX. */
		physx::PxPhysics* _physxSDK;

		/** Puntero al cocinado de PhysX. */
		physx::PxCooking* _cooking;

		/** Puntero al gestor de colisiones */
		CCollisionManager* _collisionManager;

		/** True si el actor representa a un trigger. */
		bool _isTrigger;

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
