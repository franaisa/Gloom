/**
@file IronHellGoat.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CIronHellGoatFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "IronHellGoatFeedback.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CIronHellGoatFeedback);

	//__________________________________________________________________

	CIronHellGoatFeedback::CIronHellGoatFeedback() : IWeaponFeedback("ironHellGoat") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CIronHellGoatFeedback::~CIronHellGoatFeedback() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CIronHellGoatFeedback::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IWeaponFeedback::spawn(entity, map, entityInfo) ) return false;

		return true;
	}

	//__________________________________________________________________

	void CIronHellGoatFeedback::primaryFire() {
		// Poner la animacion de carga inestable del arma
	}

	//__________________________________________________________________

	void CIronHellGoatFeedback::stopPrimaryFire() {
		// Parar la animacion de carga
	}

	//__________________________________________________________________

	void CIronHellGoatFeedback::secondaryFire() {
		// De momento ná pisha
	}

}//namespace Logic