/**
@file ShootProjectile.cpp

Contiene la implementación del componente que abstrae las particularidades
del disparo de proyectiles.
 
@see Logic::CShootProjectile
@see Logic::IComponent
*/

#include "ShootProjectile.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"


namespace Logic {
	//IMP_FACTORY(CShootProjectile);

	CShootProjectile::CShootProjectile() {
		// Nada que hacer
	}

	//__________________________________________________________________

	CShootProjectile::~CShootProjectile() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CShootProjectile::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShoot::spawn(entity, map, entityInfo)) return false;

		// Leer los parametros que toquen para los proyectiles
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;	////!!!! Aqui debes de poner el nombre del arma que leera en el map.txt
		std::string weapon = aux.str();

		_projectileRadius = entityInfo->getFloatAttribute(weapon + "ProjectileRadius");

		if(entityInfo->hasAttribute("audioNoAmmo"))
			_noAmmo = entityInfo->getStringAttribute("audioNoAmmo");

		if(entityInfo->hasAttribute(weapon+"Audio"))
			_audioShoot = entityInfo->getStringAttribute(weapon+"Audio");

		return true;
	}
	//__________________________________________________________________

	// Implementa el patrón template, las clases derivadas solo deben
	// preocuparse de implementar el método "fireWeapon".
	void CShootProjectile::primaryShoot() {
		if(_primaryCanShoot && _currentAmmo > 0 && _numberShots <= _currentAmmo){
			_primaryCanShoot = false;
			_primaryCooldownTimer = 0;
				
			drawParticle("fire", "shootParticle");

			for(int i = 0; i < _numberShots; ++i)
				fireWeapon();

			decrementAmmo();

			//Sonido de disparo
			emitSound(_audioShoot, "audioShot");
		}
		else if(_currentAmmo == 0) {
			// Ejecutar sonidos y animaciones de falta de balas
			emitSound(_noAmmo, "noAmmo", true);
		}
	}
	//__________________________________________________________________

	void CShootProjectile::secondaryShoot() {

	}
	//__________________________________________________________________

} // namespace Logic

