//---------------------------------------------------------------------------
// StaticEntity.h
//---------------------------------------------------------------------------

/**
@file StaticEntity.h

Contiene la declaración de la clase que representa a las entidades físicas
estáticas.

@see Physics::CStaticEntity

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_StaticEntity_H
#define __Physics_StaticEntity_H

#include "BaseSubsystems/Math.h"
#include "Physics/Entity.h"
#include "GeometryFactory.h"
#include "MaterialManager.h"

#include <vector>
#include <string>

namespace Physics {

	/**
	Clase que representa a las entidades físicas estáticas.

	IMPORTANTE: Actualmente solo permitimos la carga de actores que
	se componen de un solo shape. En el futuro convendría cambiar esto
	ya que un mismo actor puede estar compuesto de varios shapes distintos.

	@ingroup physicsGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CStaticEntity : public CEntity {
	public:


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
		Crea un plano estático a partir de los parámetros dados.

		@param plane Geometría del plano que queremos crear.
		@param material Material que queremos que tenga el plano.
		@param group Grupo de colisión del plano.
		@param groupList Grupos de colisión con los que queremos que interactue el plano.
		@param component Componente lógico asociado.
		*/
		void load(const PlaneGeometry& plane, Material& material, int group,
				  const std::vector<int>& groupList, const Logic::IPhysics* component);

		//________________________________________________________________________

		/** 
		Crea una entidad estática a partir de los parámetros dados.

		@param position Posicion en la que queremos crear la entidad. Notar que una vez
		seteada, ya no podemos moverla de posición.
		@param geometry Geometría de la entidad estática. Por ser una entidad estática
		soporta cualquier geometría.
		@param material Material del que queremos que esté formada la entidad.
		@param trigger True si es un trigger.
		@param group Grupo de colisión de la entidad.
		@param groupList Grupos de colisión con los que queremos que la entidad interactue.
		@param component Componente lógico asociado.
		*/
		void load(const Matrix4& transform, const Geometry& geometry, Material& material, 
				  bool trigger, int group, const std::vector<int>& groupList, const Logic::IPhysics* component);

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
