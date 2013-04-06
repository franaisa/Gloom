//---------------------------------------------------------------------------
// DynamicEntity.h
//---------------------------------------------------------------------------

/**
@file DynamicEntity.h

Contiene la declaración de la clase que representa a las entidades físicas
dinámicas.

@see Physics::CDynamicEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_DynamicEntity_H
#define __Physics_DynamicEntity_H

#include "BaseSubsystems/Math.h"
#include "Physics/Entity.h"

#include <geometry/PxGeometry.h>
#include <PxMaterial.h>
#include <PxForceMode.h>

#include <vector>
#include <string>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace physx {
	class PxRigidDynamic;
}

namespace Physics {

	// Enumerado para los tipos de fuerza que se le pueden aplicar
	// a una entidad dinámica.
	enum ForceMode {
		eFORCE				= physx::PxForceMode::eFORCE,
		eIMPULSE			= physx::PxForceMode::eIMPULSE,
		eVELOCITY_CHANGE	= physx::PxForceMode::eVELOCITY_CHANGE,
		eACCELERATION		= physx::PxForceMode::eACCELERATION
	};

	/**
	Clase que representa a las entidades físicas dinámicas.

	IMPORTANTE: Las entidades físicas tienen orientación (a diferencia de
	los controllers). Actualmente solo soportamos métodos de seteo de posición.
	Deberiamos permitir setear la orientación también. Además de esto, solo
	permitimos la carga de un shape por actor, esto debería cambiar en el futuro.

	@ingroup physicsGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CDynamicEntity : public CEntity {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CDynamicEntity::CDynamicEntity();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CDynamicEntity();


		// =======================================================================
		//                     METODOS HEREDADOS DE CEntity
		// =======================================================================


		/**
		Carga una entidad física desde un fichero RepX.

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisión que queremos asignar al actor.
		@param groupList Grupos de colisión con los que el actor quiere interactuar.
		@param component Componente lógico asociado.
		*/
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics *component);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Método que inicializa la entidad física. Actualmente asignamos por defecto
		una sola shape a la entidad (definida por la geometría y densidad dadas).
		En realidad, una entidad física puede estar formada por varias shapes, por
		lo que es previsible que en el futuro cambiemos este método para que sea
		aún más genérico.

		@param position Posición donde queremos crear la entidad física.
		@param geometry Geometría que queremos que tenga la entidad física. 
		Importante: PhysX solo soporta ciertas geometrias para los dinámicos.
		@param material Material físico que aplicaremos a la entidad.
		@param density Densidad de la geometría que vamos a asignar a la entidad.
		@param kinematic True si la entidad es kinemática.
		@param trigger True si la entidad es un trigger.
		@param group Grupo de colisión del actor.
		@param groupList Grupos de colisión con los que queremos que la entidad interactue.
		@param component Componente lógico asociado.
		*/
		void load(const Vector3 &position, const physx::PxGeometry& geometry, physx::PxMaterial& material, 
				  float density, bool kinematic, bool trigger, int group, 
				  const std::vector<int>& groupList, const Logic::IPhysics* component);

		//________________________________________________________________________
						 
		/**
		Empuja a la entidad física en una dirección, con una fuerza y de una cierta manera.

		@param forceVector Vector de fuerza, típicamente = vectorDirector * magnitudFuerza
		@param forceMode Modo de empuje, por defecto eFORCE (empuje lineal).
		@param autowake True si queremos que el actor se despierte al ser empujado (si estaba
		durmiendo).
		*/
		void addForce(const Vector3& forceVector, ForceMode forceMode = ForceMode::eFORCE, bool autowake = true);

		//________________________________________________________________________

		/**
		Empuje rotatorio en una dirección, con una fuerza y de una cierta manera.

		@param forceVector Vector de fuerza, típicamente = vectorDirector * magnitudFuerza
		@param forceMode Modo de empuje, por defecto eFORCE (empuje lineal).
		@param autowake True si queremos que el actor se despierte al ser empujado (si estaba
		durmiendo).
		*/
		void addTorque(const Vector3& forceVector, ForceMode forceMode = ForceMode::eFORCE, bool autowake = true);

		//________________________________________________________________________

		/**
		Desactiva la gravedad para esta entidad.

		IMPORTANTE: Si ejecutamos este método durante la simulación física, no se
		despertarán los actores a los que afecte este cambio (por razones de 
		eficiencia según PhysX). En ese caso tenemos que despertarlos de forma
		manual.

		@param state True si queremos desactivar la gravedad.
		*/
		void disableGravity(bool state);

		//________________________________________________________________________

		/**
		Método para saber si la entidad física es kinemática.

		@return True si la entidad es kinemática.
		*/
		bool isKinematic();

		//________________________________________________________________________

		/**
		Método para mover a la entidad física.

		IMPORTANTE: Para poder mover el actor éste debe ser kinemático, en caso contrario
		no funciona.

		@param transform Posición y orientación objetivo que queremos tener.
		*/
		void move(const Matrix4 &transform);

		//________________________________________________________________________

		/**
		Método para mover a la entidad física.

		IMPORTANTE: Para poder mover el actor éste debe ser kinemático, en caso contrario
		no funciona.

		@param transform Posición objetivo que queremos tener.
		*/
		void move(const Vector3& displ);

		//________________________________________________________________________

		/** 
		Setea la posición de la entidad física. Mucho cuidado porque este método
		lo que hace es transportar a la entidad (no moverla).

		IMPORTANTE: Actualmente solo tenemos un set position, pero las entidades
		física TIENEN orientación. Sería útil tener un metodo de colocación que
		oriente también a la entidad física.

		@param position Posicion a donde queremos transportar la entidad.
		@param makeConversionToLogicWorld True si queremos que la entidad física
		se coloque en la posición dada + el desfase entre pivote lógico y físico.
		*/
		void setPosition(const Vector3 &position, bool makeConversionToLogicWorld);

		/** 
		Setea el transform de la entidad física. Mucho cuidado porque este método
		lo que hace es transportar a la entidad (no moverla).

		@param transform Transform al que queramos convertir la entidad la entidad.
		@param makeConversionToLogicWorld True si queremos que la entidad física
		se coloque en la posición dada + el desfase entre pivote lógico y físico.
		*/
		void setTransform(const Matrix4 &transform, bool makeConversionToLogicWorld);


		/** 
		Setea el transform de la entidad física. Mucho cuidado porque este método
		lo que hace es transportar a la entidad (no moverla).

		@param position Position del que queremos recalcular el pivote
		@return Posición nueva ponderada en la Y para el pivote
		*/
		Vector3 CDynamicEntity::TransformPositionLogicPhysX(Vector3 position);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Actor dinámico. */
		physx::PxRigidDynamic* _dynamicActor;

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
