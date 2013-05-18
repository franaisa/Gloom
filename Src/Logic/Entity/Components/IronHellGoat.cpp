/**
@file IronHellGoat.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CIronHellGoat
@see Logic::CShootProjectile

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "IronHellGoat.h"

namespace Logic {
	IMP_FACTORY(CIronHellGoat);

	//__________________________________________________________________

	CIronHellGoat::~CIronHellGoat() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CIronHellGoat::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !CShootProjectile::spawn(entity, map, entityInfo) ) return false;

		return true;
	}

	//__________________________________________________________________

	void CIronHellGoat::primaryShoot() {
		std::cout << "primario" << std::endl;
	}

	//__________________________________________________________________

	void CIronHellGoat::secondaryShoot() {
		std::cout << "secundario" << std::endl;
	}

}//namespace Logic