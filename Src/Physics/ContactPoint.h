/**
@file ContactPoint.h

Contiene la declaración del struct que contiene
información sobre el punto de contacto entre
dos shapes.

@see Logic::CContactPoint

@author Francisco Aisa García.
@date Abril, 2013.
*/

#ifndef __Physics_ContactPoint_H
#define __Physics_ContactPoint_H

#include "Conversions.h"
#include "BaseSubsystems/Math.h"

namespace Logic {
	class CEntity;
	class IPhysics;
}

namespace Physics {

	/**
	Este struct condensa los atributos más relevantes
	cuando se produce un impacto entre dos shapes.

	@author Francisco Aisa García
	@date Abril, 2013
	*/

	struct CContactPoint {
		/** Impulso aplicado en el punto de impacto. Divido por el deltaTime es la fuerza que se aplica. */
		Vector3 impulse;
		/** Posicion del impacto. */
		Vector3 position;
		/** Normal del impacto */
		Vector3 normal;
		/** Separación entre shapes. Separación negativa indica solapamiento de shapes. */
		float separation;

		CContactPoint() : impulse(Vector3::ZERO), position(Vector3::ZERO), normal(Vector3::ZERO), separation(0.0f) { }
	}; 

} // namespace Physics

#endif 
