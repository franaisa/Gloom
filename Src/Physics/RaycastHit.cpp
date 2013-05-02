/**

@author Francisco Aisa García
@date Abril, 2013
*/

#include "RaycastHit.h"
#include "Logic/Entity/Entity.h"

namespace Physics {

	CRaycastHit::CRaycastHit() {
		// Nada que hacer
	}

	CRaycastHit::CRaycastHit(Logic::CEntity* entity, float distance, const Vector3& impact, const Vector3& normal) : 
		entity(entity), distance(distance), impact(impact), normal(normal) {
	
		// Nada que hacer
	}

	CRaycastHit::CRaycastHit(const CRaycastHit& rhs) : entity(rhs.entity), 
													   distance(rhs.distance),
													   impact(rhs.impact),
													   normal(rhs.normal) {
	
	}

	CRaycastHit::~CRaycastHit() {
		// Nada que destruir
	}

	CRaycastHit& CRaycastHit::operator=(const CRaycastHit& rhs) {
		if(this != &rhs) {
			entity = rhs.entity;
			distance = rhs.distance;
			impact = rhs.impact;
			normal = rhs.normal;
		}

		return *this;
	}

} // namespace Physics
