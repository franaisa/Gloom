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
		*/
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component) = 0;

		//________________________________________________________________________

		/** Devuelve la posición y rotación de la entidad física. */
		Matrix4 getTransform() const;

		//________________________________________________________________________

		/** Activa la simulación física. */
		void activateSimulation();

		//________________________________________________________________________

		/** Desactiva la simulación física. */
		void deactivateSimulation();


	protected:


		// =======================================================================
		//                          METODOS PRIVADOS
		// =======================================================================


		/** 
		Método para construir un actor de PhysX a partir de un fichero RepX.

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisión que queremos asignar al actor.
		@param groupList Grupos de colisión con los que el actor quiere interactuar.
		@param component Componente lógico asociado.
		*/
		physx::PxRigidActor* deserializeFromRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component);

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
