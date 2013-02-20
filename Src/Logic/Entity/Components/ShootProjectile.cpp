/**
@file ShootProjectile.cpp

Contiene la implementación del componente que abstrae las particularidades
del disparo de proyectiles.
 
@see Logic::CShootProjectile
@see Logic::IComponent
*/

#include "ShootProjectile.h"

namespace Logic {
	//IMP_FACTORY(CShootProjectile);
	
	bool CShootProjectile::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShoot::spawn(entity,map,entityInfo)) return false;

		// Leer los parametros que toquen para los proyectiles

		return true;
	}

	//__________________________________________________________________

	// Implementa el patrón template, las clases derivadas solo deben
	// preocuparse de implementar el método "fireWeapon".
	void CShootProjectile::shoot() {
		if(_canShoot && _currentAmmo > 0 && _numberShots <= _currentAmmo){
			_canShoot = false;
			_cooldownTimer = 0;
				
			for(int i = 0; i < _numberShots; ++i)
				fireWeapon();

			decrementAmmo();
		}
		else if(_currentAmmo == 0) {
			// Ejecutar sonidos y animaciones de falta de balas
			//triggerRunOutOfAmmoMessages();
		}
	}

} // namespace Logic

