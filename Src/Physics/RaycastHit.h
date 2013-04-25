/**
@file GeometryFactory.h

Contiene la declaración de la factoría de geometrías
físicas.

@see Logic::CGeometryFactory

@author Francisco Aisa García.
@date Abril, 2013.
*/

#ifndef __Physics_RaycastHit_H
#define __Physics_RaycastHit_H

#include "BaseSubsystems/Math.h"

// La factoria devuelve tipos estaticos (nada de punteros)
// por ello incluimos las cabeceras y no hacemos predeclaraciones

namespace Logic {
	class CEntity;
}

namespace Physics {

	/**
	Factoría de geometrías físicas. Los actores están formados de shapes, que
	a su vez están formados por geometrías. La factoría nos da la posibilidad
	de crear actores y hacer queries especificando la geometría que queremos.

	@author Francisco Aisa García
	@date Abril, 2013
	*/

	class CRaycastHit {
	public:
		CRaycastHit();
		CRaycastHit(Logic::CEntity* entity, float distance, const Vector3& impact, const Vector3& normal);
		CRaycastHit(const CRaycastHit& rhs);
		~CRaycastHit();

		CRaycastHit& operator=(const CRaycastHit& rhs);

		// Miembros
		Logic::CEntity* entity;
		float distance;
		Vector3 impact;
		Vector3 normal;
	}; 

} // namespace Physics

#endif 
