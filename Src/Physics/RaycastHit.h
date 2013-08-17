/**
@file RaycastHit.h

Contiene la declaración del struct que contiene
información del hit de un raycast de físicas.

@see Logic::CRaycastHit

@author Francisco Aisa García.
@date Abril, 2013.
*/

#ifndef __Physics_RaycastHit_H
#define __Physics_RaycastHit_H

#include "BaseSubsystems/Math.h"

namespace Logic {
	class CEntity;
	class IPhysics;
}

namespace Physics {

	/**
	Este struct condensa los atributos más relevantes
	devueltos por la query de raycast de físicas.

	@author Francisco Aisa García
	@date Abril, 2013
	*/

	struct CRaycastHit {
		/** Puntero a la entidad golpeada. */
		Logic::CEntity* entity;
		/** Puntero al componente fisico de la entidad golpeada. */
		Logic::IPhysics* physicComponent;
		/** Nombre del collider golpeado, unknown en el caso de no tener un nombre asignado.*/
		std::string colliderName;
		/** Distancia desde el punto de salida del ray hasta el golpeo. */
		float distance;
		/** Punto de impacto del raycast. */
		Vector3 impact;
		/** Normal de impacto del raycast. */
		Vector3 normal;

		CRaycastHit(): entity(0), distance(-1), impact( Vector3::ZERO ), normal( Vector3::ZERO ) {};
	}; 

} // namespace Physics

#endif 
